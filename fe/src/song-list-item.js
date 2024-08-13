import { css, html, LitElement } from "lit";

import "./icon-button.js";
import { SettingsMixin } from "./mixin-settings.js";
import { WaveformUtilMixin } from "./mixin-waveform-util.js";
import { } from "./star-rating.js";


class SongListItem extends SettingsMixin(WaveformUtilMixin(LitElement)) {
  render() {
    let comments = !this.settings.getBool('songList.showComments') ? '' :
                       this.song.comments.search(/warn/i) == -1 ? this.song.comments : html`<span class="warn">${this.song.comments}</span>`;
    let bgImg = 'background-image: url("file://' + this.settings.getString('dir.music') + this.song.album.coverFilepath + '")';
    let waveforms = '';
    if (this.settings.getBool('songList.showWaveforms')) {
      let wfImg = 'mask-image:url("';
      if (this.song.trashed) {
        wfImg += '../resources/no-waveform.webp';
      } else {
        wfImg += 'file://' + this.getFullWaveformFilepath(this.song.filepath);
      }
      wfImg += '");background-color: ' + this.waveformColorFilter();
      waveforms = html`
        <div id="waveform" style="${wfImg}" @click="${this.previewSong}"></div>
        <icon-button icon="send" @click="${this.dragSongToDeckA}" style="transform: scaleX(-1)"></icon-button>
        <icon-button icon="send" @click="${this.dragSongToDeckB}"></icon-button>
      `;
    }
    let inPlaylist = this.playlists.some(p => p.query === "");
    return html`
      <div class="song-item" draggable="true" @dragstart="${this.dragSong}" @click="${this.selectSong}" @drop="${this.handleDrop}" @dragover="${this.handleDragOver}" @dragleave="${this.handleDragLeave}">
        ${waveforms}
        <div id="cover" style="${bgImg}"></div>
        <div class="key fade-out">
          <span class="artist">${this.song.artist}</span>
          <span> - </span>
          <span class="title">${this.song.title}</span>
          <span class="comments">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;${comments}</span>
        </div>
        ${inPlaylist ? html`<icon-button icon="backspace" @click="${this.removeSongFromPlaylist}"></icon-button>` : html``}
        ${!!this.song.dupeId ? html`<icon-button icon="flip_to_back" @click="${this.showDupeId}"></icon-button>` : html``}
        <icon-button icon="edit" @click="${this.openEditSongPage}"></icon-button>
        <div>${this.computeBpmShift(this.song, this.bpm)}</div>
        ${this.settings.getBool('songList.column.bpm') ? html`<div>${this.song.bpm}</div>` : html``}
        <div class="energy-${this.song.energy}">${this.song.energy}</div>
        <div>${this.computeDuration(this.song)}</div>
        <star-rating value="${this.song.rating}" class="${this.trashedClass(this.song.trashed)}" readonly></star-rating>
        <time class="added">${this.computeLocaleDateString(this.song)}</time>
      </div>
    `;
  }

  static get properties() {
    return {
      bpm: { type: Number },
      mvRestrict: { type: Boolean },
      useStems: { type: Boolean },
      playlists: { type: Array },
      song: { type: Object },
    };
  }

  constructor() {
    super();
    this.playlists = [];
  }

  selectSong(e) {
    let grp = e.metaKey || e.shiftKey || e.ctrlKey;
    let event = new CustomEvent('select-song', {
        detail: { song: this.song, multi: grp, shift: !!e.shiftKey },
        bubbles: true,
        composed: true });
    this.dispatchEvent(event);
  }

  computeBpmShift(song, bpm) {
    if (!bpm) return '';
    let songBpm = !!song ? song.bpm : undefined;
    if (!songBpm) return 'n/a';
    if (Math.abs(songBpm - bpm) > Math.abs(songBpm/2 - bpm)) songBpm = songBpm / 2;
    else if (Math.abs(songBpm - bpm) > Math.abs(songBpm * 2 - bpm)) songBpm = songBpm * 2;
    return ((bpm / songBpm - 1) * 100).toFixed(2);
  }

  async reloadWaveform() {
    let style = this.shadowRoot.getElementById('waveform').style;
    style.WebkitMaskBoxImage = 'url("file://' + this.getFullWaveformFilepath(this.song.filepath) + '?forceReload=1")';
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
    e.preventDefault();
    let filepath = '';
    let iconpath = '';
    if (this.mvRestrict) {
      filepath = this.settings.getString('dir.mv') + this.song.musicVideo.filePath;
      iconpath = this.settings.getString('dir.mv') + this.song.musicVideo.thumbnailFilePath;
    } else {
      filepath = this.settings.getString('dir.music') + this.song.filepath;
      iconpath = this.settings.getString('dir.music') + this.song.album.coverFilepath;
      if (this.useStems) {
        let stemFilepath = this.settings.getString('dir.stems') + this.song.filepath.replace(/\.[^.]+$/, '.stem.m4a');
        ipcRenderer.invoke('existsfilepath', stemFilepath)
        .then((exists) => {
          if (exists) {
            filepath = stemFilepath;
          }
          ipcRenderer.send('ondragstart', filepath, iconpath);
          window.ssDraggedObj = this.song;
        }).catch((err) => {
          this.addAlert("Failed checking for existence of stems file.\n" + err, 8);
        });
        return;
      }
    }

    ipcRenderer.send('ondragstart', filepath, iconpath);
    window.ssDraggedObj = this.song;
  }

  handleDragLeave(e) {
    e.preventDefault();
  }

  handleDragOver(e) {
    e.preventDefault();
  }

  handleDrop(e) {
    if (this.playlists.length == 1 && this.playlists[0].query === "" &&
        this.song.id != window.ssDraggedObj.id) {
      let before = false;
      {
        let rect = this.getBoundingClientRect();
        if (e.clientY < rect.y || e.clientY > rect.y + rect.height)
          console.error('Drop location y (' + e.clientY + ') was out of range of landing element (' + rect.y + ' to ' + (rect.y + rect.height) + ').');
        if (e.clientY < rect.y + rect.height / 2) before = true;
        console.log(rect.y + ', ' + e.clientY);
      }
      let playlist = this.playlists[0];
      let song = window.ssDraggedObj;
      let posChange = 0;
      let newPosition = ss.PlaylistEntry.findByPlaylistIdAndSongId(playlist.id,  this.song.id).position + (before ? 0 : 1);
      for (let entry of ss.PlaylistEntry.findByPlaylistId(playlist.id)) {
        if (before && entry.song.id == this.song.id) posChange += 1;
        if (entry.song.id == song.id) {
          posChange -= 1;
          entry.position = newPosition;
        } else {
          entry.position = entry.position + posChange;
        }
        if (!before && entry.song.id == this.song.id) posChange += 1;
        entry.update();
      }
    }
    let event = new CustomEvent('search', {
        bubbles: true,
        composed: true });
    this.dispatchEvent(event);
    e.preventDefault();
  }

  removeSongFromPlaylist(e) {
    for (let p of this.playlists.filter(p => p.query === "")) {
      let entry = ss.PlaylistEntry.findByPlaylistIdAndSongId(p.id, this.song.id);
      if (!!entry) entry.erase();
    }
    let event = new CustomEvent('search', {
        bubbles: true,
        composed: true });
    this.dispatchEvent(event);
    e.stopPropagation();
  }

  showDupeId(e) {//ss-add-alert
    let event = new CustomEvent('ss-add-alert', {
        detail: {msg: `Duplicate Song ID :  ${this.song.dupeId}`, timeoutInSeconds: 3},
        bubbles: true,
        composed: true });
    this.dispatchEvent(event);
    e.stopPropagation();
  }

  openEditSongPage(e) {
    let event = new CustomEvent('song-edit', { detail: {song: this.song } });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  dragSongToDeckA(e) {
    this.dragSongTo(this.settings.getString('dragAndDrop.deckLeftX'), this.settings.getString('dragAndDrop.deckLeftY'));
    e.stopPropagation();
    setTimeout(() => this.selectSong(e), 1000);  // wait to select song until after drag event
  }

  dragSongToDeckB(e) {
    this.dragSongTo(this.settings.getString('dragAndDrop.deckRightX'), this.settings.getString('dragAndDrop.deckRightY'));
    e.stopPropagation();
    setTimeout(() => this.selectSong(e), 1000);  // wait to select song until after drag event
  }

  dragSongTo(x, y) {
    const util = require('util');
    const exec = util.promisify(require('node:child_process').exec);
    const easing = this.settings.getInt('dragAndDrop.easing');
    const wait = this.settings.getInt('dragAndDrop.waitTimeInMs');

    let dnd = async () => {
      exec(`cliclick -e ${easing} -r m:. w:10 dd:. w:10 dm:${x},${y} w:${wait} du:${x},${y} w:50`);
    }
    dnd();
  }

  previewSong(e) {
    let bounds = this.shadowRoot.getElementById('waveform').getBoundingClientRect();
    let pct = (e.clientX - bounds.x) / bounds.width;
    let event = new CustomEvent('audio-preview-song', {
        bubbles: true,
        composed: true,
        detail: { song: this.song, pct: pct, player: this }
    });
    this.dispatchEvent(event);
    e.stopPropagation();
  }

  waveformColorFilter() {
    if (!!this.song.energy) {
      return `var(--ss-song-list-item-waveform-${this.song.energy})`
    } else {
      return 'var(--ss-song-list-item-waveform-0)'
    }
  }

  static get styles() {
    return [
      css`
        :host([selected]) .song-item {
          background-color: var(--ss-song-list-item-hover-background-color);
        }
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
          display: block;
          --mdc-icon-size: 16px;
          padding: 0 3px;
          visibility: hidden;
        }
        .song-item:hover > icon-button {
          visibility: visible;
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
        :host([selected]) .song-item .fade-out:after {
          background: var(--ss-song-list-item-hover-fade-out);
        }

        #cover {
          background-position: center center;
          background-size: cover;
          height: 30px;
          width: 30px;
        }

        #waveform {
          mask-position: center center;
          mask-size: contain;
          mask-repeat: no-repeat;
          background-color: var(--ss-song-list-item-artist-color);
          filter: brightness(0.5) saturate(2);
          height: 24px;
          width: 200px;
          padding: 0 3px;
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
