import { css, html, LitElement } from "lit";

import "@material/mwc-fab/mwc-fab.js";

import "./playlist-list-item.js";
import { PlaylistsMixin } from "./mixin-playlist.js";


class PlaylistList extends PlaylistsMixin(LitElement) {
  render() {
    let items = this.allPlaylists.map(p => html`<playlist-list-item .playlist="${p}" @toggle-select="${this.toggleSelection}" ?selected="${this.isPlaylistSelected(p)}"></playlist-list-item>`);
    return html`
      ${items}
      <mwc-fab mini icon="add" @click="${this.createPlaylist}"></mwc-fab>
    `;
  }

  static get properties() {
    return {
      allPlaylists: { type: Array },
    }
  }

  constructor() {
    super();
    this.allPlaylists = ss.Playlist.findAll().sort((a, b) => a.name.localeCompare(b.name));
    this.updatePlaylistsListener = (e) => {
      this.allPlaylists = ss.Playlist.findAll().sort((a, b) => a.name.localeCompare(b.name));
      this.requestUpdate();
    };
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('update-playlists', this.updatePlaylistsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('update-playlists', this.updatePlaylistsListener);
    super.disconnectedCallback();
  }

  createPlaylist(e) {
    var playlist = new ss.Playlist();
    let event = new CustomEvent('playlist-edit', { detail: playlist });
    window.dispatchEvent(event);
  }

  isPlaylistSelected(p) {
    for (let i = 0; i < this.playlists.length; ++i) {
      if (this.playlists[i].id == p.id) return true;
    }
    return false;
  }

  toggleSelection(e) {
    this.togglePlaylist(e.detail.playlist, e.detail.multi);
  }

  static get styles() {
    return [
      css`
        :host {
          padding: 0 0 20px 0;
          height: 100%;
          overflow-x: hidden;
          overflow-y: scroll;
        }
        mwc-fab {
          bottom: 15px;
          position: relative;
          left: 182px;
        }
      `
    ];
  }
}

window.customElements.define('playlist-list', PlaylistList);
