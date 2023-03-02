import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-linear-progress";

import "./icon-button.js";
import { SettingsMixin } from "./mixin-settings.js";


class AudioPlayer extends SettingsMixin(LitElement) {
  render() {
    let progress = this.currentTime / this.duration;
    return html`
      <icon-button id="playPause" icon="play_arrow" @click="${this.playAction}"></icon-button>
      <div class="timebar">
        <div id="time" self-end>${this.currentTimeStr()}</div>
        <mwc-linear-progress id="progress" determinate progress="${progress}" buffer="1" @click="${this.changeCurrentTime}"></mwc-linear-progress>
      </div>
    `;
  }

  static get properties() {
    return {
      song: { type: Object },
      src: { type: String },
      prevSongId: {
        type: Number,
        attribute: false
      },
      duration: {
        type: Number,
        attribute: false
      },
      currentTime: {
        type: Number,
        attribute: false
      },
    }
  }

  constructor() {
    super();
    this.fs = require('fs');
    this.duration = 1;
    this.currentTime = 0;
    this.prevSongId = 0;

    this.durationChangeListener = () =>  {
      this.duration = this.audio.duration;
    };
    this.endedListener = () => {
      this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
    };
    this.errorListener = () => {
      console.error('Audio error: ' + mediaError.code);
    };
    this.timeUpdateListener = () => {
      this.currentTime = this.audio.currentTime;
    };
  }

  disconnectedCallback() {
    this.destroyAudio();
    super.disconnectedCallback();
  }

  attributeChangedCallback(name, oldval, newval) {
    super.attributeChangedCallback(name, oldval, newval);
    if (name == 'src') this.srcChanged(newval);
  }

  destroyAudio() {
    if (!!this.audio) {
      this.audio.removeEventListener('durationchange', this.durationChangeListener);
      this.audio.removeEventListener('ended', this.endedListener);
      this.audio.removeEventListener('error', this.errorListener);
      this.audio.removeEventListener('timeupdate', this.timeUpdateListener);
    }
    this.audio = null;
  }

  createAudio() {
    if (!!this.src) {
      this.audio = new Audio(this.src);
      this.audio.addEventListener('durationchange', this.durationChangeListener);
      this.audio.addEventListener('ended', this.endedListener);
      this.audio.addEventListener('error', this.errorListener);
      this.audio.addEventListener('timeupdate', this.timeUpdateListener);
    }
  }

  srcChanged(newValue) {
    this.currentTime = 0;
    this.song = null;
    this.prevSongFilepath = '';
    this.destroyAudio();
    this.createAudio();
  }

  playAction(e) {
    if (this.shadowRoot.getElementById('playPause').icon === 'play_arrow') {
      this.play();
    } else {
      this.pause();
    }
  }

  async possiblyCreate() {
    if (!!this.song && this.song.filepath != this.prevSongFilepath) {
      this.destroyAudio();
      this.prevSongFilepath = this.song.filepath;
      // audio source; use local, otherwise contact google api
      let filepath = this.song.filepath.charAt(0) === '/' ? this.song.filepath : this.settings.getString('music.dir') + this.song.filepath;
      let promiseAccess = (filepath) => new Promise((resolve) => this.fs.access(filepath, this.fs.F_OK | this.fs.R_OK, resolve));
      let err = await promiseAccess(filepath);
      if (!err) {
        this.src = 'file://' + filepath;
        this.createAudio();
      } else {
        if (!this.song.youtubeId) {
          console.warn('Unable to play song ' + this.song.id + ' because it does not have a YouTube song ID associated with it.');
        } else {
          let url = await window.gpm.getStreamUrl(this.song.youtubeId);
          this.src = url;
          this.createAudio();
        }
      }
    }
  }

  play() {
    let play = async () => {
      try {
        await this.possiblyCreate();
        this.audio.play();
        this.shadowRoot.getElementById('playPause').icon = 'pause';
      } catch(err) {
        console.warn('Unable to play song.');
        console.warn(err);
      }
    };
    play();
  }

  pause() {
    if (!!this.audio) {
      this.audio.pause();
      this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
    }
  }

  currentTimeStr() {
    // set the current time in readable string format
    let str = '';
    let time = this.currentTime|0;
    let hours = Math.floor(time / 3600);
    if (hours) str += hours + ':';
    time %= 3600;
    let mins = Math.floor(time / 60);
    if (hours || mins) str += (mins < 10 && hours ? '0' : '') + mins + ':';
    let secs = time % 60;
    str += (secs < 10 && (hours || mins) ? '0' : '') + secs;
    return str;
  }

  changeCurrentTime(e) {
    let bounds = this.shadowRoot.getElementById('progress').getBoundingClientRect();
    let pct = (e.clientX - bounds.x) / bounds.width;
    this.audio.currentTime = pct * this.duration;
  }

  static get styles() {
    return [
      css`
        :host {
          margin: 4px 0;
          display: flex;
          flex-direction: row;
        }
        .timebar {
          flex: 1 1 auto;
          display: flex;
          flex-direction: column;
          margin-left: 6px;
        }
      `,
    ];
  }
}

window.customElements.define('audio-player', AudioPlayer);
