import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let playlistMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      playlist: { type: Object }
    };
  }

  constructor() {
    super();
    this._playlistListener = (e) => this._playlistChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('playlist-changed', this._playlistListener);
  }

  disconnectedCallback() {
    window.removeEventListener('playlist-changed', this._playlistListener);
    super.disconnectedCallback();
  }

  changePlaylist(x) {
    let event = new CustomEvent('playlist-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _playlistChanged(e) {
    this.playlistChanged(e.detail);
  }

  playlistChanged(x) {
    this.playlist = x;
  }
}

export const PlaylistMixin = dedupingMixin(playlistMixin);
