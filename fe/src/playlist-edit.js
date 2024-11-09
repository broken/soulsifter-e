import { css, html, LitElement } from "lit";

import "@material/web/checkbox/checkbox.js";
import "@material/web/dialog/dialog.js";
import "@material/web/textfield/filled-text-field.js";

import "./abstract-action-page.js";
import "./genre-list.js";
import "./icon-button.js";
import "./search-info.js";


class PlaylistEdit extends LitElement {
  render() {
    this.spotifySync = !!this.playlist.spotifyId;
    this.youtubeSync = !!this.playlist.youtubeId;
    return html`
      <div class="mainContainer">
        <ss-googleplaymusic id="gpm"></ss-googleplaymusic>
        <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
          <div class="fields">
            <md-filled-text-field label="Name" .value="${this.playlist.name}" id="name"></md-filled-text-field>
            <div class="query-container">
              <md-filled-text-field label="Query" .value="${this.playlist.query}" id="query"></md-filled-text-field>
              <icon-button @click=${this.openSearchInfoDialog} icon="info_outline" id="info-btn"></icon-button>
            </div>
            <br>
            <label><md-checkbox ?checked="${this.fromSongTrail}" id="trail"></md-checkbox>from Song Trail</label>
            <label><md-checkbox ?checked="${this.spotifySync}" id="spotifySync" disabled></md-checkbox>sync with Spotify</label>
            <label><md-checkbox ?checked="${this.youtubeSync}" id="youtubeSync"></md-checkbox>sync with YouTube</label>
          </div>
          <div class="genres">
            <genre-list id="genreList" .genres="${this.genres}"></genre-list>
          </div>
          <md-dialog id="searchInfoDialog">
            <search-info slot="content"></search-info>
          </md-dialog>
        </abstract-action-page>
        <div class="metadata">
          <div style="user-select:text">${this.playlist.id}</div>
        </div>
      </div>
    `;
  }

  static get properties() {
    return {
      playlist: { type: Object },
      fromSongTrail: { type: Boolean },
      spotifySync: { type: Boolean },
      youtubeSync: { type: Boolean },
      genres: { type: Array },
    }
  }

  constructor() {
    super();
    this.fromSongTrail = false;
    this.spotifySync = false;
    this.youtubeSync = false;
    this.playlist = new ss.Playlist();
    this.genres = [];
    this.openPageListener = (e) => {
      if (e.detail) {
        this.playlist = ss.Playlist.findById(e.detail.id);
        this.fromSongTrail = false;
        this.spotifySync = !!this.playlist.spotifyId;
        this.youtubeSync = !!this.playlist.youtubeId;
        this.genres = this.playlist.styles;
      }
      this.classList.add('show');
    };
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('playlist-edit', this.openPageListener);
  }

  disconnectedCallback() {
    window.removeEventListener('playlist-edit', this.openPageListener);
    super.disconnectedCallback();
  }

  save(e) {
    this.playlist.name = this.shadowRoot.getElementById('name').value;
    this.playlist.query = this.shadowRoot.getElementById('query').value;
    this.fromSongTrail = this.shadowRoot.getElementById('trail').checked;
    this.spotifySync = this.shadowRoot.getElementById('spotifySync').checked;
    this.youtubeSync = this.shadowRoot.getElementById('youtubeSync').checked;
    // Do I need to store in an intermediate array?
    let styleIds = this.genres.map(function(g) { return g.id; });
    this.playlist.styleIds = styleIds;

    if (this.playlist.id) {
      this.playlist.update();
    } else {
      this.playlist.save();
      if (this.fromSongTrail) {
        let event = new CustomEvent('save-song-trail', { detail: this.playlist });
        window.dispatchEvent(event);
      }
    }
    if (this.spotifySync && !this.playlist.spotifyId) {
      window.spotify.createPlaylist(this.playlist.id);
    }
    if (this.youtubeSync && !this.playlist.youtubeId) {
      window.yt.createPlaylist(this.playlist.id);
    }
    let event = new CustomEvent('update-playlists', { });
    window.dispatchEvent(event);
    this.exit();
  }

  exit(e) {
    this.classList.remove('show');
  }

  openSearchInfoDialog(e) {
    this.shadowRoot.getElementById('searchInfoDialog').show();
  }

  static get styles() {
    return [
      css`
        :host {
          font-size: 16px;
          display: none;
          position: absolute;
          top: 0;
          right: 0;
          bottom: 0;
          left: 0;
          z-index: 10;
        }
        :host(.show) {
          display: block;
        }
        .genres {
          width: 240px;
          height: 360px;
        }
        .query-container {
          position: relative;
        }
        #info-btn {
          position: absolute;
          top: 0;
          right: 0;
        }
        #searchInfoDialog {
          max-width: none;
        }
        .metadata {
          display: flex;
          flex-direction: row;
          position: absolute;
          align-content: flex-end;
          bottom: 0;
          right: 0;
          left: 0;
          font-size: 13px;
          color: #777777;  /* TODO put into theme */
        }
        .mainContainer {
          display: relative;
          height: 100%;
          width: 100%;
        }
        md-checkbox {
          margin-right: 10px;
        }
        md-filled-text-field {
          width: 100%;
        }
      `,
    ];
  }
}

window.customElements.define('playlist-edit', PlaylistEdit);
