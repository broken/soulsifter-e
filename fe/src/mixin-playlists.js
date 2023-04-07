import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let playlistsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      playlists: { type: Array }
    };
  }

  constructor() {
    super();
    this._playlistListener = (e) => this._playlistsChanged(e);
    this.playlists = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('playlists-changed', this._playlistsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('playlists-changed', this._playlistsListener);
    super.disconnectedCallback();
  }

  changePlaylists(x) {
    let event = new CustomEvent('playlists-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _playlistsChanged(e) {
    this.playlistsChanged(e.detail);
  }

  playlistsChanged(x) {
    this.playlists = x;
  }

  togglePlaylist(p, isMulti) {
    for (let i = 0; i < this.playlists.length; ++i) {
      if (this.playlists[i].id == p.id) {
        this.playlists.splice(i, 1);
        this.changePlaylists(this.playlists);
        return;
      }
    }
    if (isMulti) this.playlists.push(p);
    else this.playlists = [p];
    this.changePlaylists(this.playlists);
  }
}

export const PlaylistsMixin = dedupingMixin(playlistsMixin);
