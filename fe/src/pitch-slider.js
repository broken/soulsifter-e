import { css, html, LitElement } from "lit";

import "@material/web/progress/linear-progress.js";
import "@material/web/textfield/filled-text-field.js";
import { Debouncer } from '@polymer/polymer/lib/utils/debounce.js';
import { timeOut } from '@polymer/polymer/lib/utils/async.js';

import { BpmMixin } from "./mixin-bpm.js";
import { SongMixin } from "./mixin-song.js";


class PitchSlider extends SongMixin(BpmMixin(LitElement)) {
  render() {
    let lowVal = this.pitch > 0 ? 1 : 1 - this.pitch / this.pitchMin;
    let highVal = this.pitch < 0 ? 0 : this.pitch / this.pitchMax;
    return html`
      <md-linear-progress class="reverse" value="${lowVal}" @click="${this.negativeClickAction}" id="low"></md-linear-progress>
      <md-linear-progress value="${highVal}" @click="${this.positiveClickAction}" id="high"></md-linear-progress>
      <md-filled-text-field .value="${this.pitch.toString()}" id="val" readonly></md-filled-text-field>
    `;
  }

  static get properties() {
    return {
      pitchMax: { type: Number },
      pitchMin: { type: Number },
    }
  }

  constructor() {
    super();
    this.pitch = 0;
    this.bpm = 0;
    this.song = new ss.Song();
    this.pitchMin = -8;
    this.pitchMax = 8;
    this.userPitchChange = true;
  }

  positiveClickAction(e) {
    let bounds = this.shadowRoot.getElementById('high').getBoundingClientRect();
    let pct = (e.clientX - bounds.x) / bounds.width;
    this.pitchChanged(pct * this.pitchMax);
  }

  negativeClickAction(e) {
    let bounds = this.shadowRoot.getElementById('low').getBoundingClientRect();
    let pct = 1 - (e.clientX - bounds.x) / bounds.width;
    this.pitchChanged(pct * this.pitchMin);
  }

  songChanged(song) {
    this.song = song;
    if (!song || !song.bpm) return;
    if (!this.bpm) {
      this.changeBpm(parseFloat(song.bpm).toFixed(1));
    } else {
      this.updatePitch();
    }
  }

  bpmChanged(bpm) {
    this.bpm = bpm;
    this.updatePitch();
  }

  pitchChanged(pitch) {
    this.pitch = pitch;
    if (this.userPitchChange && !!this.song && !!this.song.bpm) {
      this.changeBpm((this.song.bpm * (this.pitch/100) + this.song.bpm * 1).toFixed(1));
      this.userPitchChange = false;
    }
    // we debounce the reset here since changing the bpm can cause the pitchChanged observer to be called multiple times
    this.debouncer = Debouncer.debounce(
        this.debouncer,  // initially undefined
        timeOut.after(50),
        () => this.userPitchChange = true);
  }

  updatePitch() {
    if (!this.bpm || !this.song || !this.song.bpm) return;
    let songBpm = this.song.bpm;
    if (Math.abs(songBpm - this.bpm) > Math.abs(songBpm/2 - this.bpm)) songBpm = songBpm / 2;
    else if (Math.abs(songBpm - this.bpm) > Math.abs(songBpm * 2 - this.bpm)) songBpm = songBpm * 2;
    let pitch = ((this.bpm / songBpm - 1) * 100).toFixed(2);
    //if (pitch > this.pitchMax) pitch = this.pitchMax;
    //if (pitch < this.pitchMin) pitch = this.pitchMin;
    this.pitch = pitch;
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: row;
          align-items: center;
        }
        md-linear-progress {
          min-width: 20px;
          width: 67px;
          --md-linear-progress-active-indicator-color: var(--ss-pitch-slider-color);
          --md-linear-progress-track-color: var(--ss-pitch-slider-background-color);
        }
        md-linear-progress.reverse {
          --md-linear-progress-active-indicator-color: var(--ss-pitch-slider-background-color);
          --md-linear-progress-track-color: var(--ss-pitch-slider-color);
        }
        md-filled-text-field {
          margin-left: 12px;
          width: 35px;
          --md-filled-text-field-bottom-space: 0;
          --md-filled-text-field-top-space: 0;
        }
      `,
    ];
  }
}

window.customElements.define('pitch-slider', PitchSlider);
