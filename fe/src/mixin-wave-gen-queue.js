import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let waveGenQueueMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      waveGenQueue: { type: Array }
    };
  }

  constructor() {
    super();
    this._waveGenQueueListener = (e) => this._waveGenQueueChanged(e);
    this._waveGenCompletedListener = (e) => this._waveGenCompleted(e);
    this.waveGenQueue = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('waveGenQueue-changed', this._waveGenQueueListener);
    window.addEventListener('waveGen-completed', this._waveGenCompletedListener);
  }

  disconnectedCallback() {
    window.removeEventListener('waveGenQueue-changed', this._waveGenQueueListener);
    window.removeEventListener('waveGen-completed', this._waveGenCompletedListener);
    super.disconnectedCallback();
  }

  // Normally this is in the public API, but I made it easier with push, next, & clear.
  _changeWaveGenQueue(x) {
    let event = new CustomEvent('waveGenQueue-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _waveGenQueueChanged(e) {
    this.waveGenQueueChanged(e.detail);
  }

  // This should never be publicly called, only publicly overridden
  waveGenQueueChanged(x) {
    this.waveGenQueue = x;
  }

  notifyWaveGenCompleted(x) {
    let event = new CustomEvent('waveGen-completed', { detail: x });
    window.dispatchEvent(event);
  }

  _waveGenCompleted(e) {
    this.waveGenCompleted(e.detail);
  }

  // To be overridden
  waveGenCompleted(x) {
  }

  pushSongToWaveGenQueue(song) {
    this.waveGenQueue.push(song.filepath);
    this._changeWaveGenQueue(this.waveGenQueue);
  }

  removeSongFromWaveGenQueue() {
    let filepath = this.waveGenQueue.shift();
    this._changeWaveGenQueue(this.waveGenQueue);
    return filepath;
  }

  clearWaveGenQueue() {
    this._changeWaveGenQueue([]);
  }
}

export const WaveGenQueueMixin = dedupingMixin(waveGenQueueMixin);
