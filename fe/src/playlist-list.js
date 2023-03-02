import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-fab/mwc-fab.js";

import "./playlist-list-item.js";
import { PlaylistMixin } from "./mixin-playlist.js";


class PlaylistList extends PlaylistMixin(LitElement) {
  render() {
    let items = this.playlists.map(p => html`<playlist-list-item .playlist="${p}" @toggle-select="${this.toggleSelection}" ?selected="${!!this.playlist && this.playlist.id == p.id}"></playlist-list-item>`);
    return html`
      ${items}
      <mwc-fab mini icon="add" @click="${this.createPlaylist}"></mwc-fab>
    `;
  }

  static get properties() {
    return {
      playlists: { type: Array },
    }
  }

  constructor() {
    super();
    this.playlists = ss.Playlist.findAll().sort((a, b) => a.name.localeCompare(b.name));
    this.updatePlaylistsListener = (e) => {
      this.playlists = ss.Playlist.findAll().sort((a, b) => a.name.localeCompare(b.name));
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

  toggleSelection(e) {
    this.playlist = !!this.playlist && this.playlist.id == e.detail.playlist.id ? null : e.detail.playlist;
    this.changePlaylist(this.playlist);
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
