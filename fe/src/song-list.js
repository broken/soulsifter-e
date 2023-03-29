// <link href="../iron-signals/iron-signals.html" rel="import">
// <link href="../core-tooltip/core-tooltip.html" rel="import">

// <link href="song-list-item.html" rel="import">
// <link href="ss-global-behavior.html" rel="import">

import { css, html, LitElement } from "lit";

import { AlertsMixin } from "./mixin-alerts.js";
import { BpmMixin } from "./mixin-bpm.js";
import { GenresMixin } from "./mixin-genres.js";
import { PlaylistMixin } from "./mixin-playlist.js";
import { QueryMixin } from "./mixin-query.js";
import { SearchMixin } from "./mixin-search.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongEditMixin } from "./mixin-song-edit.js";
import { SongMixin } from "./mixin-song.js";
import { SongTrailMixin } from "./mixin-song-trail.js";
import { } from "./song-list-item.js";

class SongList extends AlertsMixin(BpmMixin(GenresMixin(PlaylistMixin(QueryMixin(SearchMixin(SearchOptionsMixin(SettingsMixin(SongEditMixin(SongMixin(SongTrailMixin(LitElement))))))))))) {
  render() {
    let songListItems = html``;
    if (this.entries.length) songListItems = this.entries.map(e => html`<song-list-item .song="${e.song}" .playlistEntry="${e}" bpm="${this.bpm}" @select-song="${this.selectSong}" ?mvRestrict="${this.searchOptions.mvRestrict}"></song-list-item>`);
    else songListItems = this.songs.map(s => html`<song-list-item .song="${s}" bpm="${this.bpm}" @select-song="${this.selectSong}" ?mvRestrict="${this.searchOptions.mvRestrict}"></song-list-item>`);
    return html`
      ${songListItems}
    `;
  }
  static get properties() {
    return {
      songs: { type: Array },
      entries: { type: Array },  // saved to prevent from being garbage collected
      song: { type: Object },
      genres: { type: Array },
      mvRestrict: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.genres = [];
    this.entries = [];
    this.songTrail = [];
    this.search();
    this.mvRestrict = false;
  }

  bpmChanged(bpm) {
    this.bpm = bpm;
    if (this.searchOptions.bpmRestrict) this.search();
    else this.shadowRoot.querySelectorAll('song-list-item').forEach(el => el.bpm = this.bpm);
  }

  playlistChanged(playlist) {
    this.playlist = playlist;
    if (!!playlist) this.search();
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
    this.changeSong(e.detail.song);
  }

  searchOptionsChanged(opts) {
    this.searchOptions = opts;
    if (this.searchOptions.mvRestrict != this.mvRestrict) {
      this.mvRestrict = this.searchOptions.mvRestrict;
      this.shadowRoot.querySelectorAll('song-list-item').forEach(el => el.mvRestrict = this.mvRestrict);
    }
  }

  updateEditedSong(s) {
    this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
      if (el.song.id == s.id) el.song = s;
    });
  }

  search() {
    if (!!this.playlist) {
      if (!this.playlist.query) {
        this.entries = ss.PlaylistEntry.findByPlaylistId(this.playlist.id);
        this.entries = this.entries.sort(function(a, b) { return a.position - b.position; });
        this.songs = this.entries.map(function(x) { return x.song; });
        return;
      } else {
        this.query = this.playlist.query;
        this.genres = this.playlist.styles;
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
    this.songs = ss.SearchUtil.searchSongs(p.q, p.bpm, p.keys, this.genres, omitSongs, this.settings.getInt('songList.limit'), p.energy, this.searchOptions.mvRestrict, this.searchOptions.orderBy, (msg) => { this.addAlert(msg, 5); });
  }

  static get styles() {
    return [
      css`
        :host {
          overflow-x: hidden;
          overflow-y: scroll;
        }
      `
    ];
  }
}

window.customElements.define('song-list', SongList);
