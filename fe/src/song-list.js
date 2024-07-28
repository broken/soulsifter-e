// <link href="../iron-signals/iron-signals.html" rel="import">
// <link href="../core-tooltip/core-tooltip.html" rel="import">

// <link href="song-list-item.html" rel="import">
// <link href="ss-global-behavior.html" rel="import">

import { css, html, LitElement } from "lit";

import "@material/web/button/filled-button.js";
import "@material/web/button/filled-tonal-button.js";
import "@material/web/checkbox/checkbox.js";
import "@material/web/dialog/dialog.js";
import "@material/web/textfield/filled-text-field.js";
// Features of paper-dialog over md-dialog: noCancelOnOutsideClick noAutoFocus verticalAlign="bottom" verticalOffset="8"
import "@polymer/paper-dialog/paper-dialog.js";

import "./icon-button.js";
import { AlertsMixin } from "./mixin-alerts.js";
import { BpmMixin } from "./mixin-bpm.js";
import { GenresMixin } from "./mixin-genres.js";
import { MidiMixin } from "./mixin-midi.js";
import { PlaylistsMixin } from "./mixin-playlists.js";
import { QueryMixin } from "./mixin-query.js";
import { SearchMixin } from "./mixin-search.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongEditMixin } from "./mixin-song-edit.js";
import { SongMixin } from "./mixin-song.js";
import { SongTrailMixin } from "./mixin-song-trail.js";
import { WaveGenQueueMixin } from "./mixin-wave-gen-queue.js";
import { WaveformUtilMixin } from "./mixin-waveform-util.js";
import { } from "./song-list-item.js";

class SongList extends AlertsMixin(
                       BpmMixin(
                       GenresMixin(
                       MidiMixin(
                       PlaylistsMixin(
                       QueryMixin(
                       SearchMixin(
                       SearchOptionsMixin(
                       SettingsMixin(
                       SongEditMixin(
                       SongMixin(
                       SongTrailMixin(
                       WaveGenQueueMixin(
                       WaveformUtilMixin(
                       LitElement
)))))))))))))) {
  render() {
    let songListItems = html``;
    songListItems = this.songs.map(s => html`<song-list-item .song="${s}" .playlists="${this.playlists}" bpm="${this.bpm}" @select-song="${this.selectSong}" @search="${this.search}" ?mvRestrict="${this.searchOptions.mvRestrict}" ?useStems="${this.searchOptions.useStems}"></song-list-item>`);
    const fields = ['artist', 'comments', 'curator']
    let dialogs = fields.map(f => html`
        <md-dialog id="${f}">
            <md-filled-text-field label="${f.split('_').map(w => w.charAt(0).toUpperCase() + w.slice(1)).join(' ')}" id="${f + '_input'}" slot="content"></md-filled-text-field>
            <div class="editActions" slot="actions">
              <md-filled-tonal-button @click="${this.cancelEdit(f)}">Cancel</md-filled-tonal-button>
              <md-filled-button @click="${this.saveEdit(f)}">Accept</md-filled-button>
            </div>
          </md-dialog>`);
    dialogs.push(['is_mixed'].map(f => html`
        <md-dialog id="${f}">
            <label slot="content"><md-checkbox id="${f + '_input'}"></md-checkbox>${f.split('_').map(w => w.charAt(0).toUpperCase() + w.slice(1)).join(' ')}</label>
            <div class="editActions" slot="actions">
              <md-filled-tonal-button @click="${this.cancelEdit(f)}">Cancel</md-filled-tonal-button>
              <md-filled-button @click="${this.saveEdit(f)}">Accept</md-filled-button>
            </div>
          </md-dialog>`));
    dialogs.push(['add_genres', 'replace_genres'].map(f => html`
        <md-dialog id="${f}">
            <genre-list id="${f + '_input'}" singleselect slot="content"></genre-list>
            <div class="editActions" slot="actions">
              <md-filled-tonal-button @click="${this.cancelEdit(f)}">Cancel</md-filled-tonal-button>
              <md-filled-button @click="${this.saveEdit(f)}">Accept</md-filled-button>
            </div>
          </md-dialog>`));
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
          <options-menu-item @click="${this.markDuplicates}">mark as duplicates</options-menu-item>
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
    this.midiSelectedListItem = undefined;
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

  midiConnected(myChan) {
    myChan.addListener('noteon', e => {
      console.log(e);
      const loadA = 70;
      const loadB = 71;
      if (e.message.dataBytes[0] != loadA && e.message.dataBytes[0] != loadB) return;
      if (!this.midiSelectedListItem) return;

      let rect = this.midiSelectedListItem.getBoundingClientRect();
      const chromeOffset = 31;  // TODO setting or compute with mouse listen
      const ex = '=' + ((rect.left + window.screenX + rect.width / 2) | 0);
      const ey = '=' + ((rect.top + window.screenY + chromeOffset + rect.height / 2) | 0);

      let x = 0;
      let y = 0;

      ipcRenderer.send('ss-focus');
      if (e.message.dataBytes[0] == loadA) {
        x = this.settings.getString('dragAndDrop.deckLeftX');
        y = this.settings.getString('dragAndDrop.deckLeftY');
      } else {
        x = this.settings.getString('dragAndDrop.deckRightX');
        y = this.settings.getString('dragAndDrop.deckRightY');
      }

      const util = require('util');
      const exec = util.promisify(require('node:child_process').exec);
      const easing = this.settings.getInt('dragAndDrop.easing');
      const wait = this.settings.getInt('dragAndDrop.waitTimeInMs');

      let dnd = async () => {
        exec(`cliclick -e ${easing} -r m:${ex},${ey} w:100 dd:. w:10 dm:${x},${y} w:${wait} du:${x},${y} w:50`);
      }
      dnd();
      e.detail = {song: this.midiSelectedListItem.song};
      setTimeout(() => this.selectSong(e), 2000);  // wait to select song until after drag event
    });
    myChan.addListener('controlchange', e => {
      console.log(e);
      const browse = 64;
      const velRight = 1;
      const velLeft = 127;
      if (e.message.dataBytes[0] != browse) return;
      let items = this.shadowRoot.querySelectorAll('song-list-item');
      if (!this.midiSelectedListItem) {
        if (items.length) this.midiSelectedListItem = items[0];
      } else {
        this.midiSelectedListItem.removeAttribute('selected');
        if (e.message.dataBytes[1] == velRight) {
          let useNext = false;
          for (let item of items) {
            if (item == this.midiSelectedListItem) {
              useNext = true;
            } else if (useNext) {
              useNext = false;
              this.midiSelectedListItem = item;
              break;
            }
          }
        } else if (e.message.dataBytes[1] == velLeft) {
          let prevItem = items[0];
          for (let item of items) {
            if (item == this.midiSelectedListItem) {
              this.midiSelectedListItem = prevItem;
              break;
            } else {
              prevItem = item;
            }
          }
        }
      }
      this.midiSelectedListItem.setAttribute('selected', '');
      {
        // scroll so selected is in the middle
        const thisRect = this.getBoundingClientRect();
        const selectedRect = this.midiSelectedListItem.getBoundingClientRect();
        const targetTop = thisRect.height / 2 - selectedRect.height / 2 + thisRect.top;
        const moveTop = selectedRect.top - targetTop;
        const newScrollTop = this.scrollTop + moveTop;
        this.scrollTo({top: newScrollTop, behavior: 'instant'});
        console.info(`targetTop: ${targetTop}, moveTop ${moveTop}  newScrollTop: ${newScrollTop}, stop ${selectedRect.top} `);
      }
      let event = new CustomEvent('audio-preview-song', {
          bubbles: true,
          composed: true,
          detail: { song: this.midiSelectedListItem.song, pct: 0.25, player: this.midiSelectedListItem }
      });
      this.dispatchEvent(event);
    });
  }

  updateEditedSong(id) {
    this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
      if (el.song.id == id) el.song = ss.Song.findById(id);
    });
  }

  search() {
    if (this.midiSelectedListItem) {
      this.midiSelectedListItem.removeAttribute('selected');
      this.midiSelectedListItem = undefined;
    }
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
    this.generateMissingWaveforms();
  }

  async generateMissingWaveforms() {
    this.clearWaveGenQueue();
    for (let song of this.songs) {
      try {
        if (!song.trashed && !await this.hasWaveformCachedFile(song)) {
          this.pushSongToWaveGenQueue(song);
        }
      } catch (err) {
        this.addAlert('Failed checking for existence of waveforms file. ' + err, 8);
      }
    }
  }

  waveGenCompleted(fp) {
    this.shadowRoot.querySelectorAll('song-list-item').forEach(el => {
      if (el.song.filepath == fp) {
        el.reloadWaveform();
        return;
      }
    });
  }

  markDuplicates(e) {
    const duplicates = [...this.selectedListItems].map(el => el.song);
    if (duplicates.length <= 1) {
      this.addAlert('Need to select more than one song to assign duplicates.', 3);
      return;
    }
    if (duplicates.reduce((num, s) => num += !s.trashed, 0) != 1) {
      this.addAlert('All songs but one should be trashed before assigning duplicates.', 3);
      return;
    };
    const mainSong = duplicates.filter(s => !s.trashed)[0];
    const dupes = duplicates.filter(s => s.trashed);
    dupes.forEach(s => {
      s.dupeId = mainSong.id;
      s.update();
      if (s.artist != mainSong.artist ||
          s.title != mainSong.title ||
          s.rating != mainSong.rating ||
          !s.styleIds.reduce((b, g) => b && mainSong.styleIds.includes(g), true) ||
          (s.bpm && (s.bpm < mainSong.bpm - 3 || s.bpm > mainSong.bpm + 3)) ||
          (s.tonicKey && s.tonicKey != mainSong.tonicKey) ||
          (s.durationInMs && (s.durationInMs < mainSong.durationInMs - 1000 || s.durationInMs > mainSong.durationInMs + 1000))) {
        this.addAlert(`Songs are different: Artist: ${s.artist} != ${mainSong.artist} or Title: ${s.title} != ${mainSong.title} or rating: ${s.rating} != ${mainSong.rating} or Bpm: ${s.bpm} != ${mainSong.bpm} or Key: ${s.tonicKey} != ${mainSong.tonicKey} or duration: ${s.durationInMs} != ${mainSong.durationInMs} or styles: ${s.styleIds.sort().toString()} != ${mainSong.styleIds.sort().toString()}`);
      } else {
        console.log(`Songs are matched: Artist: ${s.artist} =~ ${mainSong.artist} and Title: ${s.title} =~ ${mainSong.title} and rating: ${s.rating} =~ ${mainSong.rating} and Bpm: ${s.bpm} =~ ${mainSong.bpm} and Key: ${s.tonicKey} =~ ${mainSong.tonicKey} and duration: ${s.durationInMs} =~ ${mainSong.durationInMs} and styles: ${s.styleIds.sort().toString()} =~ ${mainSong.styleIds.sort().toString()}`);
      }
      console.log('Set song ' + s.id + ' as a duplicate of song ' + mainSong.id);
    });
    this.selectedListItems.forEach(el => el.requestUpdate());
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
    return e => this.shadowRoot.getElementById(d).show();
  }

  cancelEdit(d) {
    return e => this.shadowRoot.getElementById(d).close();
  }

  saveEdit(f) {
    if (f == 'artist') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.artist = val);
        this.shadowRoot.getElementById(f).close();
      };
    } else if (f == 'comments') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.comments = val);
        this.shadowRoot.getElementById(f).close();
      };
    } else if (f == 'curator') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').value;
        this.updateSongField(s => s.curator = val);
        this.shadowRoot.getElementById(f).close();
      };
    } else if (f == 'is_mixed') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').checked;
        this.updateSongField(s => s.mixed = val);
        this.shadowRoot.getElementById(f).close();
      };
    } else if (f == 'add_genres') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').genres;
        const keys = val.map(g => g.id);
        this.updateSongField(s => s.styleIds = s.styleIds.concat(keys));
        this.shadowRoot.getElementById(f).close();
      };
    } else if (f == 'replace_genres') {
      return e => {
        const val = this.shadowRoot.getElementById(f + '_input').genres;
        const keys = val.map(g => g.id);
        this.updateSongField(s => s.styleIds = keys);
        this.shadowRoot.getElementById(f).close();
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
        }
        #multiOptionsDialog {
          color: var(--ss-song-list-multi-options-dialog-text-color);
          background-color: var(--ss-song-list-multi-options-dialog-background-color);
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
          padding: 14px 0 0 10px;
          margin: 0;
          height: 64vh;
          display: block;
          overflow: hidden;
        }
        md-checkbox {
          margin-right: 10px;
        }
        md-dialog > label {
          display: block;
        }
        md-filled-text-field {
          width: 100%;
        }
      `
    ];
  }
}

window.customElements.define('song-list', SongList);
