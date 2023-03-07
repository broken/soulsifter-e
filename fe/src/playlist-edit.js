import { css, html, LitElement } from "lit";

import "@polymer/paper-checkbox/paper-checkbox.js";
import "@polymer/paper-input/paper-input.js";

import "./abstract-action-page.js";
import "./genre-list.js";


class PlaylistEdit extends LitElement {
  render() {
    this.spotifySync = !!this.playlist.spotifyId;
    this.youtubeSync = !!this.playlist.youtubeId;
    return html`
      <ss-googleplaymusic id="gpm"></ss-googleplaymusic>
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <paper-input floatingLabel label="Name" value="${this.playlist.name}" id="name"></paper-input>
          <paper-input floatingLabel label="Query" value="${this.playlist.query}" id="query"></paper-input>
          <br>
          <paper-checkbox ?checked="${this.fromSongTrail}" id="trail">from Song Trail</paper-checkbox>
          <paper-checkbox ?checked="${this.spotifySync}" id="spotifySync" disabled>sync with Spotify</paper-checkbox>
          <paper-checkbox ?checked="${this.youtubeSync}" id="youtubeSync">sync with YouTube</paper-checkbox>
        </div>
        <div class="genres">
          <genre-list id="genreList" .genres="${this.genres}"></genre-list>
        </div>
      </abstract-action-page>
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
        this.playlist = e.detail;
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
        }
        :host(.show) {
          display: block;
        }
        .genres {
          width: 240px;
          height: 360px;
        }
      `,
    ];
  }
}

window.customElements.define('playlist-edit', PlaylistEdit);
