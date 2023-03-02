import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let songMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      song: { type: Object }
    };
  }

  constructor() {
    super();
    this._songListener = (e) => this._songChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('song-changed', this._songListener);
  }

  disconnectedCallback() {
    window.removeEventListener('song-changed', this._songListener);
    super.disconnectedCallback();
  }

  changeSong(x) {
    let event = new CustomEvent('song-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _songChanged(e) {
    this.songChanged(e.detail);
  }

  songChanged(x) {
    this.song = x;
  }
}

export const SongMixin = dedupingMixin(songMixin);
