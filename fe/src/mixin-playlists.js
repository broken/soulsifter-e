import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let playlistsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      playlists: { type: Array },
      omitPlaylists: { type: Array }
    };
  }

  constructor() {
    super();
    this._playlistsListener = (e) => this._playlistsChanged(e);
    this._omitPlaylistsListener = (e) => this._omitPlaylistsChanged(e);
    this.playlists = [];
    this.omitPlaylists = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('playlists-changed', this._playlistsListener);
    window.addEventListener('omit-playlists-changed', this._omitPlaylistsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('playlists-changed', this._playlistsListener);
    window.removeEventListener('omit-playlists-changed', this._omitPlaylistsListener);
    super.disconnectedCallback();
  }

  changePlaylists(x) {
    let event = new CustomEvent('playlists-changed', { detail: x });
    window.dispatchEvent(event);
  }

  changeOmitPlaylists(x) {
    let event = new CustomEvent('omit-playlists-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _playlistsChanged(e) {
    this.playlistsChanged(e.detail);
  }

  _omitPlaylistsChanged(e) {
    this.omitPlaylistsChanged(e.detail);
  }

  playlistsChanged(x) {
    this.playlists = x;
  }

  omitPlaylistsChanged(x) {
    this.omitPlaylists = x;
  }

  togglePlaylist(p, isMulti) {
    const omitIndex = this.omitPlaylists.findIndex(op => op.id == p.id);
    if (omitIndex !== -1) {
      this.omitPlaylists.splice(omitIndex, 1);
      this.changeOmitPlaylists(this.omitPlaylists);
    }

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

  toggleOmitPlaylist(p, isMulti) {
    const playlistIndex = this.playlists.findIndex(pl => pl.id == p.id);
    if (playlistIndex !== -1) {
      this.playlists.splice(playlistIndex, 1);
      this.changePlaylists(this.playlists);
    }

    for (let i = 0; i < this.omitPlaylists.length; ++i) {
      if (this.omitPlaylists[i].id == p.id) {
        this.omitPlaylists.splice(i, 1);
        this.changeOmitPlaylists(this.omitPlaylists);
        return;
      }
    }
    if (isMulti) this.omitPlaylists.push(p);
    else this.omitPlaylists = [p];
    this.changeOmitPlaylists(this.omitPlaylists);
  }

  isPlaylistSelected(p) {
    return !!(this.playlists && this.playlists.some(pl => pl.id == p.id));
  }

  isPlaylistExcluded(p) {
    return !!(this.omitPlaylists && this.omitPlaylists.some(op => op.id == p.id));
  }
}

export const PlaylistsMixin = dedupingMixin(playlistsMixin);
