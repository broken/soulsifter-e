import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let audioVolumeMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      audioVolume: { type: Number }  // 0 - 1
    };
  }

  constructor() {
    super();
    this.audioVolume = 1;
    this._audioVolumeListener = (e) => this._audioVolumeChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('audio-volume-changed', this._audioVolumeListener);
  }

  disconnectedCallback() {
    window.removeEventListener('audio-volume-changed', this._audioVolumeListener);
    super.disconnectedCallback();
  }

  changeAudioVolume(x) {
    let event = new CustomEvent('audio-volume-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _audioVolumeChanged(e) {
    this.audioVolumeChanged(e.detail);
  }

  audioVolumeChanged(x) {
    this.audioVolume = x;
  }
}

export const AudioVolumeMixin = dedupingMixin(audioVolumeMixin);
