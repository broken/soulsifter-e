import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let songTrailMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      songTrail: { type: Array }
    };
  }

  constructor() {
    super();
    this._songTrailListener = (e) => this._songTrailChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('song-trail-changed', this._songTrailListener);
  }

  disconnectedCallback() {
    window.removeEventListener('song-trail-changed', this._songTrailListener);
    super.disconnectedCallback();
  }

  changeSongTrail(x) {
    let event = new CustomEvent('song-trail-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _songTrailChanged(e) {
    this.songTrailChanged(e.detail);
  }

  songTrailChanged(x) {
    this.songTrail = x;
  }
}

export const SongTrailMixin = dedupingMixin(songTrailMixin);
