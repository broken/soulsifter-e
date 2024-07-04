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
    this.waveGenQueue = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('waveGenQueue-changed', this._waveGenQueueListener);
  }

  disconnectedCallback() {
    window.removeEventListener('waveGenQueue-changed', this._waveGenQueueListener);
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

  getWaveGenQueueImageFilepath(song) {
    return this.settings.getString('dir.waveforms') + song.filepath.replace(/\.[^.]+$/, '.webp');
  }

  getWaveGenQueueImageFilepathFromSongPath(filepath) {
    return this.settings.getString('dir.waveforms') + filepath.replace(/\.[^.]+$/, '.webp');
  }

  async hasWaveGenQueueImageFilepath(song) {
    let f = this.getWaveGenQueueImageFilepath(song);
    return ipcRenderer.invoke('existsfilepath', f);
  }
}

export const WaveGenQueueMixin = dedupingMixin(waveGenQueueMixin);
