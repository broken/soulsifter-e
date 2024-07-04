import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let wavesurferMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      wavesurferQueue: { type: Array },
      wavesurferQueueCheck: { type: Object }
    };
  }

  constructor() {
    super();
    this._wavesurferListener = (e) => this._wavesurferChanged(e);
    this.wavesurferQueue = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('wavesurfer-changed', this._wavesurferListener);
  }

  disconnectedCallback() {
    window.removeEventListener('wavesurfer-changed', this._wavesurferListener);
    super.disconnectedCallback();
  }

  // Normally this is in the public API, but I made it easier with push, next, & clear.
  _changeWavesurferQueue(x) {
    let event = new CustomEvent('wavesurfer-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _wavesurferChanged(e) {
    this.wavesurferQueueChanged(e.detail);
  }

  // This should never be publicly called, only publicly overridden
  wavesurferQueueChanged(x) {
    this.wavesurferQueue = x;
  }

  pushSongToWavesurferQueue(song) {
    this.wavesurferQueue.push(song.filepath);
    this._changeWavesurferQueue(this.wavesurferQueue);
  }

  removeSongFromWavesurferQueue() {
    let filepath = this.wavesurferQueue.shift();
    this._changeWavesurferQueue(this.wavesurferQueue);
    return filepath;
  }

  clearWavesurferQueue() {
    this._changeWavesurferQueue([]);
  }

  getWavesurferImageFilepath(song) {
    return this.settings.getString('dir.waveforms') + song.filepath.replace(/\.[^.]+$/, '.webp');
  }

  getWavesurferImageFilepathFromSongPath(filepath) {
    return this.settings.getString('dir.waveforms') + filepath.replace(/\.[^.]+$/, '.webp');
  }

  async hasWavesurferImageFilepath(song) {
    let f = this.getWavesurferImageFilepath(song);
    return ipcRenderer.invoke('existsfilepath', f);
  }
}

export const WavesurferMixin = dedupingMixin(wavesurferMixin);
