/**
 * HTML controls of an audio-player.
 *
 * This doesn't actually play audio, but sends events to trigger the audio
 * player on an ancestor component.
 */

import { css, html, LitElement } from "lit";

import "@material/web/progress/linear-progress.js";

import "./icon-button.js";
import { SettingsMixin } from "./mixin-settings.js";


class AudioPlayer extends SettingsMixin(LitElement) {
  render() {
    return html`
      <icon-button id="playPause" icon="play_arrow" @click="${this.playAction}"></icon-button>
      <div class="timebar">
        <div id="time" self-end>${this.currentTimeStr()}</div>
        <md-linear-progress value="${this.progress}" @click="${this.changeCurrentTime}" id="progress"></md-linear-progress>
      </div>
    `;
  }

  static get properties() {
    return {
      song: { type: Object },
      src: { type: String },
      progress: { type: Number },
      currentTime: { type: Number },
      prevSongId: {
        type: Number,
        attribute: false
      },
    }
  }

  constructor() {
    super();
    this.progress = 0;
    this.currentTime = 0;
    this.prevSongId = 0;
  }

  attributeChangedCallback(name, oldval, newval) {
    super.attributeChangedCallback(name, oldval, newval);
    if (name == 'src') this.srcChanged(newval);
  }

  srcChanged(newValue) {
    this.currentTime = 0;
    this.song = null;
    this.prevSongFilepath = '';
  }

  playAction(e) {
    if (this.shadowRoot.getElementById('playPause').icon === 'play_arrow') {
      this.play();
    } else {
      this.pause();
    }
  }

  play() {
    let event = new CustomEvent(
        'audio-play',
        { bubbles: true,
          composed: true,
          detail: {song: this.song, src: this.src, player: this}
        }
    );
    this.dispatchEvent(event);
    this.shadowRoot.getElementById('playPause').icon = 'pause';
  }

  pause() {
    this.dispatchEvent(new CustomEvent('audio-pause', {bubbles: true, composed: true}));
    this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
  }

  audioEnded() {
    this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
    let event = new CustomEvent('song-ended', { bubbles: true, composed: true });
    this.dispatchEvent(event);
  }

  audioUnload() {
    this.shadowRoot.getElementById('playPause').icon = 'play_arrow';
    this.progress = 0;
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
    let event = new CustomEvent(
        'audio-set-time-pct',
        { bubbles: true, composed: true, detail: {pct: pct}}
    );
    this.dispatchEvent(event);
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
        icon-button {
          color: var(--ss-audio-player-button-color);
        }
      `,
    ];
  }
}

window.customElements.define('audio-player', AudioPlayer);
