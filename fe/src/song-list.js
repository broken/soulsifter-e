// <link href="../iron-signals/iron-signals.html" rel="import">
// <link href="../core-tooltip/core-tooltip.html" rel="import">

// <link href="song-list-item.html" rel="import">
// <link href="ss-global-behavior.html" rel="import">

import { css, html, LitElement } from "@polymer/lit-element";

import { AlertsMixin } from "./mixin-alerts.js";
import { BpmMixin } from "./mixin-bpm.js";
import { GenresMixin } from "./mixin-genres.js";
import { PlaylistMixin } from "./mixin-playlist.js";
import { QueryMixin } from "./mixin-query.js";
import { SearchMixin } from "./mixin-search.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongMixin } from "./mixin-song.js";
import { SongTrailMixin } from "./mixin-song-trail.js";
import { } from "./song-list-item.js";

class SongList extends AlertsMixin(BpmMixin(GenresMixin(PlaylistMixin(QueryMixin(SearchMixin(SearchOptionsMixin(SettingsMixin(SongMixin(SongTrailMixin(LitElement)))))))))) {
  render() {
    let songListItems = this.songs.map(s => html`<song-list-item .song="${s}"></song-list-item>`);
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
      searchOptionBpmRestrict: { type: Boolean },
      searchOptionKeyRestrict: { type: Boolean },
      searchOptionTrashedRestrict: { type: Boolean },
      searchOptionRepeatRestrict: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.genres = [];
    this.entries = [];
    this.songTrail = [];
    this.search();
  }

  bpmChanged(bpm) {
    this.bpm = bpm;
    if (this.searchOptions.bpmRestrict) this.search();
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

  search() {
    if (!!this.playlist) {
      if (!this.playlist.query) {
        this.entries = ss.PlaylistEntry.findByPlaylistId(this.playlist.id);
        this.songs = this.entries.sort(function(a, b) { return a.position - b.position; })
                                 .map(function(x) { return x.song; });
        return;
      } else {
        this.entries = [];
        this.query = this.playlist.query;
        this.genres = this.playlist.styles;
      }
    }
    let genreIds = this.genres.map(g => g.id);
    let p = {q: this.query, genres: genreIds.join(',')};
    let omitSongs = [];
    p.bpm = this.searchOptions.bpmRestrict && !!this.bpm ? this.bpm : 0;
    p.keys = this.searchOptions.keyRestrict && !!this.song ? this.song.tonicKey : '';
    p.energy = this.searchOptions.energyRestrict && !!this.song ? this.song.energy : 0;
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
