import { css, html, LitElement } from "lit";

import "./icon-button.js";
import WaveSurfer from 'wavesurfer.js';
import { SettingsMixin } from "./mixin-settings.js";


class WavePlayer extends SettingsMixin(LitElement) {
  render() {
    let progress = this.currentTime / this.duration;
    return html`
      <icon-button id="playPause" icon="pause" @click="${this.playAction}" class="meta"></icon-button>
      <div id="wavesurfer"></div>
      <div id="time" class="meta">${this.currentTimeStr()}</div>
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

    // this.durationChangeListener = () =>  {
    //   this.duration = this.audio.duration;
    // };
    // this.endedListener = () => {
    //   this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
    // };
    // this.timeUpdateListener = () => {
    //   this.currentTime = this.audio.currentTime;
    // };
  }

  firstUpdated() {
    this.wavesurfer = WaveSurfer.create({
      container: this.shadowRoot.getElementById('wavesurfer'),
      height: 24
    });
    this.wavesurfer.on('error', function (msg) {
      console.error('Audio error: ' + msg);
    });
    this.songChanged();
  }

  playAction(e) {
    this.wavesurfer.playPause();
  }

  updated(changedProperties) {
    this.songChanged();
  }

  async songChanged() {
    if (!!this.song && this.song.filepath != this.prevSongFilepath) {
      this.prevSongFilepath = this.song.filepath;
      // audio source; use local, otherwise contact google api
      let filepath = this.song.filepath.charAt(0) === '/' ? this.song.filepath : this.settings.getString('dir.music') + this.song.filepath;
      let promiseAccess = (filepath) => new Promise((resolve) => this.fs.access(filepath, this.fs.F_OK | this.fs.R_OK, resolve));
      let err = await promiseAccess(filepath);
      if (!err) {
        this.src = 'file://' + filepath;
      } else {
        console.error('Unable to play song ' + this.song.id);
      }
      this.currentTime = 0;
      this.song = null;
      this.prevSongFilepath = '';
      this.wavesurfer.load(this.src);
    }
  }

  currentTimeStr() {
    return 'time';
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

  // changeCurrentTime(e) {
  //   let bounds = this.shadowRoot.getElementById('progress').getBoundingClientRect();
  //   let pct = (e.clientX - bounds.x) / bounds.width;
  //   this.audio.currentTime = pct * this.duration;
  // }

  static get styles() {
    return [
      css`
        :host {
          margin: 4px 0;
          display: flex;
          flex-direction: row;
        }
        icon-button {
          --mdc-icon-size: 24px;
        }
        .meta {
          width: 24px;
        }
        #wavesurfer {
          flex-grow: 512;
          height: 24px;
        }
      `,
    ];
  }
}

window.customElements.define('wave-player', WavePlayer);
