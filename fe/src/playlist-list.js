import { css, html, LitElement } from "lit";

import "@material/mwc-icon";
import "@material/web/fab/fab.js";

import "./playlist-list-item.js";
import { PlaylistsMixin } from "./mixin-playlists.js";


class PlaylistList extends PlaylistsMixin(LitElement) {
  render() {
    let items = this.allPlaylists.map(p => html`<playlist-list-item .playlist="${p}" @toggle-select="${this.toggleSelection}" ?selected="${this.isPlaylistSelected(p)}"></playlist-list-item>`);
    return html`
      ${items}
      <md-fab size="small" @click="${this.createPlaylist}"><mwc-icon slot="icon">add</mwc-icon></md-fab>
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
    this.requestUpdate();
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
        md-fab {
          bottom: 14px;
          position: relative;
          left: 200px;
        }
      `
    ];
  }
}

window.customElements.define('playlist-list', PlaylistList);
