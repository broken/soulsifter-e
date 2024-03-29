// <link href="../iron-signals/iron-signals.html" rel="import">
// <link href="../core-tooltip/core-tooltip.html" rel="import">

// <link href="song-list-item.html" rel="import">
// <link href="ss-global-behavior.html" rel="import">

import { css, html, LitElement } from "lit";

import "@polymer/paper-dialog/paper-dialog.js";

import "./icon-button.js";
import { AlertsMixin } from "./mixin-alerts.js";
import { BpmMixin } from "./mixin-bpm.js";
import { GenresMixin } from "./mixin-genres.js";
import { PlaylistsMixin } from "./mixin-playlists.js";
import { QueryMixin } from "./mixin-query.js";
import { SearchMixin } from "./mixin-search.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongEditMixin } from "./mixin-song-edit.js";
import { SongMixin } from "./mixin-song.js";
import { SongTrailMixin } from "./mixin-song-trail.js";
import { } from "./song-list-item.js";

class SongList extends AlertsMixin(BpmMixin(GenresMixin(PlaylistsMixin(QueryMixin(SearchMixin(SearchOptionsMixin(SettingsMixin(SongEditMixin(SongMixin(SongTrailMixin(LitElement))))))))))) {
  render() {
    let songListItems = html``;
    songListItems = this.songs.map(s => html`<song-list-item .song="${s}" .playlists="${this.playlists}" bpm="${this.bpm}" @select-song="${this.selectSong}" @search="${this.search}" ?mvRestrict="${this.searchOptions.mvRestrict}" ?useStems="${this.searchOptions.useStems}"></song-list-item>`);
    const fields = ['artist', 'comments', 'curator']
    let dialogs = fields.map(f => html`
        <paper-dialog id="${f}">
            <paper-input label="${f.split('_').map(w => w.charAt(0).toUpperCase() + w.slice(1)).join(' ')}" floatingLabel id="${f + '_input'}"></paper-input>
            <div class="editActions">
              <mwc-button @click="${this.cancelEdit(f)}" raised>Cancel</mwc-button>
              <mwc-button @click="${this.saveEdit(f)}" raised class="accent">Accept</mwc-button>
            </div>
          </paper-dialog>`);
    dialogs.push(['is_mixed'].map(f => html`
        <paper-dialog id="${f}">
            <paper-input label="${f.split('_').map(w => w.charAt(0).toUpperCase() + w.slice(1)).join(' ')}" floatingLabel id="${f + '_input'}"></paper-input>
            <div class="editActions">
              <mwc-button @click="${this.cancelEdit(f)}" raised>Cancel</mwc-button>
              <mwc-button @click="${this.saveEdit(f)}" raised class="accent">Accept</mwc-button>
            </div>
          </paper-dialog>`));
    dialogs.push(['add_genres', 'replace_genres'].map(f => html`
        <paper-dialog id="${f}">
            <genre-list id="${f + '_input'}" singleselect></genre-list>
            <div class="editActions">
              <mwc-button @click="${this.cancelEdit(f)}" raised>Cancel</mwc-button>
              <mwc-button @click="${this.saveEdit(f)}" raised class="accent">Accept</mwc-button>
            </div>
          </paper-dialog>`));
    return html`
      ${songListItems}
      <paper-dialog id="multiOptionsDialog" noCancelOnOutsideClick noAutoFocus verticalAlign="bottom" verticalOffset="8">
        <options-menu icon="edit" topright>
          <options-menu-item @click="${this.openEdit('artist')}">artist</options-menu-item>
          <options-menu-item @click="${this.openEdit('comments')}">comments</options-menu-item>
          <options-menu-item @click="${this.openEdit('is_mixed')}">is mixed</options-menu-item>
          <options-menu-item @click="${this.openEdit('curator')}">curator</options-menu-item>
          <options-menu-item @click="${this.openEdit('add_genres')}">add genres</options-menu-item>
          <options-menu-item @click="${this.openEdit('replace_genres')}">replace genres</options-menu-item>
        </options-menu>
      </paper-dialog>
      ${dialogs}
    `;
  }

  static get properties() {
    return {
      songs: { type: Array },
      entries: { type: Array },  // saved to prevent from being garbage collected
      song: { type: Object },
      genres: { type: Array },
      mvRestrict: { type: Boolean },
      useStems: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.genres = [];
    this.entries = [];
    this.songTrail = [];
    this.search();
    this.mvRestrict = false;
    this.useStems = false;
    this.selectedListItems = new Set();
    this.lastSelectedListItem = undefined;
    // used for setting back original genres after playlist selection
    this.genresBeforePlaylist = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('keydown', this.keydownHandler.bind(this));
    window.addEventListener('song-list-pos', this.getNextOrPrevSong.bind(this));
  }

  disconnectedCallback() {
    super.disconnectedCallback();
    window.removeEventListener('keydown', this.keydownHandler.bind(this));
    window.removeEventListener('song-list-pos', this.getNextOrPrevSong.bind(this));
  }

  keydownHandler(e) {
    if (e.code === 'Escape') {
      this.selectedListItems.forEach(el => el.removeAttribute('selected'));
      this.selectedListItems.clear();
      this.lastSelectedListItem = undefined;
    }
  }

  bpmChanged(bpm) {
    this.bpm = bpm;
    if (this.searchOptions.bpmRestrict) this.search();
    else this.shadowRoot.querySelectorAll('song-list-item').forEach(el => el.bpm = this.bpm);
  }

  playlistsChanged(playlists) {
    this.playlists = playlists;
    if (!!playlists.length) this.search();
    else this.genres = this.genresBeforePlaylist;
  }

  genresChanged(x) {
    this.genres = x;
    if (!this.playlists.length) this.genresBeforePlaylist = this.genres;
  }

  queryChanged(query) {
    this.query = query;
    this.search();
  }

  songChanged(song) {
    this.song = song;
    if (this.searchOptions.dynamicGenres) this.changeGenres(this.song.styles);
    if (this.settings.getBool('songList.searchOnSelect')) this.search();
  }

  selectSong(e) {
    if (e.detail.multi) {
      e.srcElement.toggleAttribute('selected');
      if (this.selectedListItems.has(e.srcElement)) {
        this.selectedListItems.delete(e.srcElement);
        if (this.lastSelectedListItem == e.srcElement) this.lastSelectedListItem = undefined;
      } else {
        if (this.selectedListItems.size == 0) {
          this.shadowRoot.getElementById('artist_input').value = e.srcElement.song.artist;
          this.shadowRoot.getElementById('comments_input').value = e.srcElement.song.comments;
          this.shadowRoot.getElementById('curator_input').value = e.srcElement.song.curator;
          this.shadowRoot.getElementById('is_mixed_input').checked = e.srcElement.song.mixed;
          this.shadowRoot.getElementById('add_genres_input').genres = [];
          this.shadowRoot.getElementById('replace_genres_input').genres = e.srcElement.song.styles;
        } else if (e.detail.shift && !!this.lastSelectedListItem) {
          let items = this.shadowRoot.querySelectorAll('song-list-item');
          let slct = false;
          for (let item of items) {
            if (item == this.lastSelectedListItem || item == e.srcElement) {
              slct = !slct;
              if (!slct) break;
            } else if (slct) {
              this.selectedListItems.add(item);
              item.setAttribute('selected', '');
            }
          }
        }
        this.selectedListItems.add(e.srcElement);
        this.lastSelectedListItem = e.srcElement;
      }
      if (this.selectedListItems.size > 0 && !this.shadowRoot.getElementById('multiOptionsDialog').opened) {
        this.shadowRoot.getElementById('multiOptionsDialog').open();
        // I have no idea why I have to explicitly set this.
        this.shadowRoot.getElementById('multiOptionsDialog').noCancelOnOutsideClick = true;
      } else if (this.selectedListItems.size == 0 && this.shadowRoot.getElementById('multiOptionsDialog').opened) {
        this.shadowRoot.getElementById('multiOptionsDialog').cancel();
      }
    } else {
      this.changeSong(e.detail.song);
      this.selectedListItems.forEach(el => el.removeAttribute('selected'));
      this.selectedListItems.clear();
      this.lastSelectedListItem = undefined;
      if (this.shadowRoot.getElementById('multiOptionsDialog').opened) {
        this.shadowRoot.getElementById('multiOptionsDialog').cancel();
      }
    }
    e.stopPropagation();
  }

  searchOptionsChanged(opts) {
    this.searchOptions = opts;
    if (this.searchOptions.mvRestrict != this.mvRestrict) {
      this.mvRestrict = this.searchOptions.mvRestrict;
      this.shadowRoot.querySelectorAll('song-list-item').forEach(el => el.mvRestrict = this.mvRestrict);
    }
    if (this.searchOptions.useStems != this.useStems) {
      this.useStems = this.searchOptions.useStems;
      this.shadowRoot.querySelectorAll('song-list-item').forEach(el => el.useStems = this.useStems);
    }
  }

  updateEditedSong(id) {
    this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
      if (el.song.id == id) el.song = ss.Song.findById(id);
    });
  }

  search() {
    let playlists = [];
    let orderBy = this.searchOptions.orderBy;
    if (!!this.playlists.length) {
      this.genres = [];
      for (let i = 0; i < this.playlists.length; ++i) {
        if (!this.playlists[i].query) {
          playlists.push(this.playlists[i]);
          orderBy = /* playlist */ 5;
        } else {
          this.query += this.playlists[i].query + " ";
          this.genres.concat(this.playlists[i].styles);
        }
      }
    }
    this.entries = [];
    let genreIds = this.genres.map(g => g.id);
    let p = {q: this.query, genres: genreIds.join(',')};
    let omitSongs = [];
    p.bpm = this.searchOptions.bpmRestrict && !!this.bpm ? Number(this.bpm) : 0;
    p.keys = this.searchOptions.keyRestrict && !!this.song ? this.song.tonicKey : '';
    p.energy = this.searchOptions.energyRestrict && !!this.song ? Number(this.song.energy) : 0;
    p.q += !this.searchOptions.trashedRestrict ? '' : (p.q.length ? ' ' : '') + 'trashed:0';
    omitSongs = !this.searchOptions.repeatRestrict ? [] : this.songTrail.map(e => e.song);
    this.songs = ss.SearchUtil.searchSongs(p.q, this.settings.getInt('songList.limit'), p.bpm, p.keys, this.genres, omitSongs, playlists, p.energy, this.searchOptions.mvRestrict, orderBy, (msg) => { this.addAlert(msg, 5); });
  }

  updateSongField(cb) {
    this.selectedListItems.forEach(el => {
      if (el.hasAttribute('selected')) {
        cb(el.song);
        el.song.update();
        el.requestUpdate();
        console.log('songs to update: ' + el.song.title);
      }
    });
  }

  openEdit(d) {
    return e => this.shadowRoot.getElementById(d).toggle();
  }

  cancelEdit(d) {
    return e => this.shadowRoot.getElementById(d).toggle();
  }

  saveEdit(f) {
    if (f == 'artist') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.artist = val);
        this.shadowRoot.getElementById(f).toggle();
      };
    } else if (f == 'comments') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.comments = val);
        this.shadowRoot.getElementById(f).toggle();
      };
    } else if (f == 'curator') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.curator = val);
        this.shadowRoot.getElementById(f).toggle();
      };
    } else if (f == 'is_mixed') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').checked;
        this.updateSongField(s => s.mixed = val);
        this.shadowRoot.getElementById(f).toggle();
      };
    } else if (f == 'add_genres') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').genres;
        const keys = val.map(g => g.id);
        this.updateSongField(s => s.styleIds = s.styleIds.concat(keys));
        this.shadowRoot.getElementById(f).toggle();
      };
    } else if (f == 'replace_genres') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').genres;
        const keys = val.map(g => g.id);
        this.updateSongField(s => s.styleIds = keys);
        this.shadowRoot.getElementById(f).toggle();
      };
    }
  }

  getNextOrPrevSong(e) {
    let songId = e.detail.songId;
    let isPrev = e.detail.isPrev;
    let isEdit = e.detail.isEdit;
    let result = undefined
    if (isPrev) {
      let prevSong = undefined
      this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
        if (el.song.id == songId) result = prevSong;
        else prevSong = el.song;
      });
    } else {
      let found = false
      this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
        if (el.song.id == songId) found = true;
        else if (found) {
          result = el.song;
          found = false;
        }
      });
    }
    if (!!result) {
      if (isEdit) {
        let event = new CustomEvent('song-edit', { detail: {song: result } });
        window.dispatchEvent(event);
      } else {
        this.changeSong(result);
      }
    }
  }

  static get styles() {
    return [
      css`
        :host {
          overflow-x: hidden;
          overflow-y: scroll;
        }
        icon-button {
          --mdc-icon-size: 24px;
          color: var(--primary-text-color);
        }
        #multiOptionsDialog {
          background-color: var(--search-toolbar-background);
          display: flex;
          flex-direction: row;
          height: 42px;
          padding: 10px;
          margin: 0px;
          bottom: 10px;
        }
        options-menu {
          margin: 0;
          padding: 0;
        }
        genre-list {
          padding: 0;
          margin: 0;
          height: 64vh;
          display: block;
          overflow: hidden;
        }
      `
    ];
  }
}

window.customElements.define('song-list', SongList);
