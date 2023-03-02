import { css, html, LitElement } from "@polymer/lit-element";

import "./icon-button.js";
import { BpmMixin } from "./mixin-bpm.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongMixin } from "./mixin-song.js";
import { } from "./star-rating.js";


class SongListItem extends BpmMixin(SearchOptionsMixin(SettingsMixin(SongMixin(LitElement)))) {
  render() {
    let comments = !this.settings.getBool('songList.showComments') ? '' :
                       this.song.comments.search(/warn/i) == -1 ? this.song.comments : html`<span class="warn">${this.song.comments}</span>`;
    let bgImg = 'background-image: url("file://' + this.settings.getString('music.dir') + this.song.album.coverFilepath + '")';
    return html`
      <div class="song-item" draggable="true" @dragstart="${this.dragSong}" @click="${this.selectSong}">
        <div id="cover" style="${bgImg}"></div>
        <div class="key fade-out">
          <span class="artist">${this.song.artist}</span>
          <span> - </span>
          <span class="title">${this.song.title}</span>
          <span class="comments">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;${comments}</span>
        </div>
        <icon-button icon="edit" @click="${this.openEditSongPage}"></icon-button>
        <div>${this.computeBpmShift(this.song, this.bpm)}</div>
        <div class="energy-${this.song.energy}">${this.song.energy}</div>
        <div>${this.computeDuration(this.song)}</div>
        <star-rating value="${this.song.rating}" class="${this.trashedClass(this.song.trashed)}" readonly></star-rating>
        <time class="added">${this.computeLocaleDateString(this.song)}</time>
      </div>
    `;
  }

  static get properties() {
    return {
    };
  }

  constructor() {
    super();
  }

  selectSong(e) {
    this.changeSong(this.song);
  }

  songChanged(song) {
    /* do nothing */
  }

  computeBpmShift(song, bpm) {
    if (!bpm) return '';
    let songBpm = !!song ? song.bpm : undefined;
    if (!songBpm) return 'n/a';
    if (Math.abs(songBpm - bpm) > Math.abs(songBpm/2 - bpm)) songBpm = songBpm / 2;
    else if (Math.abs(songBpm - bpm) > Math.abs(songBpm * 2 - bpm)) songBpm = songBpm * 2;
    return ((bpm / songBpm - 1) * 100).toFixed(2);
  }

  computeDuration(song) {
    let duration = Math.round(song.durationInMs / 1000);
    let secs = duration % 60;
    let mins = ((duration - secs) / 60) % 60;
    let hours = (duration - secs - mins * 60) / 60 / 60;
    if (secs < 10 && mins) secs = '0' + secs;
    let result = secs;
    if (mins < 10 && hours) mins = '0' + mins;
    if (mins) result = '' + mins + ':' + result;
    if (hours) result = '' + hours + ':' + result;
    return result;
  }

  trashedClass(trashed) {  /* KEEP?*/
    return trashed ? 'trashed' : '';
  }

  computeLocaleDateString(song) {
    return new Date(song.dateAdded).toLocaleDateString();
  }

  dragSong(e) {
    if (this.searchOptions.mvRestrict) {
      e.dataTransfer.setData('text/plain', 'extensis-filenames-type:' + this.settings.getString('mv.dir') + this.song.musicVideo.filePath);
    } else {
      e.dataTransfer.setData('text/plain', 'extensis-filenames-type:' + this.settings.getString('music.dir') + this.song.filepath);
    }
    window.ssDraggedObj = this.song;
  }

  openEditSongPage(e) {
    let event = new CustomEvent('song-edit', { detail: {song: this.song } });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  static get styles() {
    return [
      css`
        star-rating {
          margin: 0 6px;
        }

        .song-item {
          display: flex;
          flex-direction: row;
          align-items: center;
          height: 30px;
          padding: 0 8px;
          margin: 0 10px;
          border-bottom: var(--ss-song-list-item-border-bottom);
        }
        .song-item div {
          margin: 3px;
        }
        .song-item:hover {
          background-color: var(--ss-song-list-item-hover-background-color);
        }
        icon-button {
          display: none;
          --mdc-icon-size: 16px;
          color: var(--primary-text-color);
        }
        .song-item:hover > icon-button {
          display: block;
        }
        .song-item .artist {
          color: var(--ss-song-list-item-artist-color);
        }
        .song-item .title {
          color: var(--ss-song-list-item-title-color);
        }
        .song-item .comments {
          color: #888;
          max-width: 48px;
          display: inline-block;
        }
        .song-item .warn {
          color: #D88;
        }
        .song-item .fade-out:after {
          background: var(--ss-song-list-item-fade-out);
        }
        .song-item:hover .fade-out:after {
          background: var(--ss-song-list-item-hover-fade-out);
        }
        .song-item #menuButton {
          border: var(--ss-song-list-item-button-border);
        }
        .song-item #menuButton:hover {
          border: var(--ss-song-list-item-button-hover-border);
        }

        #cover {
          background-position: center center;
          background-size: cover;
          height: 30px;
          width: 30px;
        }

        .key {
          position: relative;
          overflow: hidden;
          white-space: nowrap;
          flex: 1 1 auto;
        }

        .energy-0 {
          color: var(--ss-song-list-item-energy-0);
        }
        .energy-1 {
          color: var(--ss-song-list-item-energy-1);
        }
        .energy-2 {
          color: var(--ss-song-list-item-energy-2);
        }
        .energy-3 {
          color: var(--ss-song-list-item-energy-3);
        }
        .energy-4 {
          color: var(--ss-song-list-item-energy-4);
        }
        .energy-5 {
          color: var(--ss-song-list-item-energy-5);
        }
        .energy-6 {
          color: var(--ss-song-list-item-energy-6);
        }
        .energy-7 {
          color: var(--ss-song-list-item-energy-7);
        }
        .energy-8 {
          color: var(--ss-song-list-item-energy-8);
        }
        .energy-9 {
          color: var(--ss-song-list-item-energy-9);
        }
        .energy-10 {
          color: var(--ss-song-list-item-energy-10);
        }

        .fade-out:after {
          display: block;
          position: absolute;
          top: 0;
          bottom: 0;
          right: 0;
          width: 30px;
          content: "";
        }

        time {
          text-align: right;
          width: 64px;
        }
      `,
    ];
  }
}

window.customElements.define('song-list-item', SongListItem);
