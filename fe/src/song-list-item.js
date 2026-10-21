import { css, html, LitElement } from "lit";

import { iconButton, iconButtonStyles } from "./icon-button.js";
import { GetFilepathMixin } from "./mixin-get-filepath.js";
import { MusicVideoMixin } from "./mixin-music-video.js";
import { SettingsMixin } from "./mixin-settings.js";
import { WaveformUtilMixin } from "./mixin-waveform-util.js";
import { starRating, starRatingStyles } from "./star-rating.js";

const { exec, path } = window.nodeAPI;


class SongListItem extends GetFilepathMixin(MusicVideoMixin(SettingsMixin(WaveformUtilMixin(LitElement)))) {
  render() {
    // performance improvement 
    const { album, comments, durationInMs, energy, trashed } = this.song;
    const { coverFilepath } = album;

    let isShort = (durationInMs / 1000) <= this.settings.getInt('songList.warning.shortSong');
    let baseComment = comments.search(/warn/i) == -1 ? comments : html`<span class="warn">${comments}</span>`;
    if (this.song.explicitLyrics) {
      baseComment = html`<span class="material-icons explicit">explicit</span>${baseComment}`;
    }
    const displayComments = !this.settings.getBool('songList.column.comments') ? '' :
               isShort ? html`${baseComment} <span class="warn">(Short)</span>` : baseComment;
    const coverPath = coverFilepath ? path.join(this.settings.getString('dir.music'), coverFilepath) : '';
    let bgImg = coverPath ? 'background-image: url("file://' + coverPath + '")' : '';
    let waveforms = '';
    if (this.settings.getBool('songList.column.waveform')) {
      let wfImg = 'mask-image:url("';
      if (trashed) {
        wfImg += '../resources/no-waveform.webp';
      } else {
        wfImg += 'file://' + this.getFullWaveformFilepath(this.song.filepath);
      }
      wfImg += '");background-color: ' + this.waveformColorFilter();
      waveforms = html`
        <div id="waveform" style="${wfImg}" @click="${this.previewSong}"></div>
      `;
    }
    let sendButtons = html`
      ${iconButton({ icon: "send", onClick: this.sendSongToDeckA, style: "transform: scaleX(-1)" })}
      ${iconButton({ icon: "send", onClick: this.sendSongToDeckB })}
    `;
    let mvIndicator = '';
    if (this.mvRestrict && this.settings.getBool('mv.on_demand')) {
      mvIndicator = (this.song && this.song.musicVideoId > 0)
        ? html`<span class="material-icons mv-indicator has-video" title="Music video available">videocam</span>`
        : html`<span class="material-icons mv-indicator no-video" title="No music video (on-demand)">videocam_off</span>`;
    }
    let inPlaylist = this.playlists.some(p => p.query === "");
    return html`
      <div class="song-item" draggable="true" @dragstart="${this.dragSong}" @click="${this.selectSong}" @drop="${this.handleDrop}" @dragover="${this.handleDragOver}" @dragleave="${this.handleDragLeave}">
        ${waveforms}
        ${this.settings.getBool('songList.column.sendButtons') ? sendButtons : html``}
        ${this.settings.getBool('songList.column.cover') ? html`<div id="cover" style="${bgImg}"></div>` : html``}
        ${mvIndicator}
        <div class="key fade-out">
          <span class="artist">${this.song.artist}</span>
          <span> - </span>
          <span class="title">${this.song.title}</span>
          <span class="comments">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;${displayComments}</span>
        </div>
        ${inPlaylist ? iconButton({ icon: "backspace", onClick: this.removeSongFromPlaylist }) : html``}
        ${!!this.song.dupeId ? iconButton({ icon: "flip_to_back", onClick: this.showDupeId }) : html``}
        ${iconButton({ className: "edit", icon: "edit", onClick: this.openEditSongPage })}
        ${this.settings.getBool('songList.column.bpmShift') ? html`<div>${this.computeBpmShift(this.song, this.bpm)}</div>` : html``}
        ${this.settings.getBool('songList.column.bpm') ? html`<div>${this.song.bpm || ''}</div>` : html``}
        ${this.settings.getBool('songList.column.energy') ? html`<div class="energy-${energy}">${energy || ''}</div>` : html``}
        ${this.settings.getBool('songList.column.duration') ? html`<div>${this.computeDuration(this.song) || ''}</div>` : html``}
        ${this.settings.getBool('songList.column.rating') ? starRating({ value: this.song.rating, className: this.trashedClass(trashed), readOnly: true }) : html``}
        ${this.settings.getBool('songList.column.releaseDate') ? html`<time class="released">${this.computeReleaseDateString(album)}</time>` : html``}
        ${this.settings.getBool('songList.column.dateAdded') ? html`<time class="added">${this.computeLocaleDateString(this.song)}</time>` : html``}
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
    if (!songBpm) return '';
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

  computeReleaseDateString(album) {
    if (!album.releaseDateYear) return '';
    const y = album.releaseDateYear;
    const m = album.releaseDateMonth;
    const d = album.releaseDateDay;
    if (!m) return `${y}`;
    const mm = m < 10 ? `0${m}` : m;
    if (!d) return `${y}-${mm}`;
    const dd = d < 10 ? `0${d}` : d;
    return `${y}-${mm}-${dd}`;
  }

  computeLocaleDateString(song) {
    if (!song.dateAdded) return '';
    const d = new Date(song.dateAdded);
    const m = d.getMonth() + 1;
    const day = d.getDate();
    return `${d.getFullYear()}-${m < 10 ? '0' + m : m}-${day < 10 ? '0' + day : day}`;
  }

  async dragSong(e) {
    e.preventDefault();
    const [filepath, iconpath] = await this.getFilepathAndIconpath(this.song, this.useStems, this.mvRestrict);
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

  async sendSongToDeckA(e) {
    e.stopPropagation();
    const shouldLoad = await this.maybeEnsureMusicVideo(this.song, this.mvRestrict);
    if (!shouldLoad) return;
    const [filepath, iconpath] = await this.getFilepathAndIconpath(this.song, this.useStems, this.mvRestrict);
    if (this.settings.getBool('virtualdj.active')) {
      window.vdj.send(`deck left load "${filepath}"`);
      this.selectSong(e);
    } else {
      this.dragSongTo(this.settings.getString('dragAndDrop.deckLeftX'), this.settings.getString('dragAndDrop.deckLeftY'));
      setTimeout(() => this.selectSong(e), 1000);  // wait to select song until after drag event
    }
  }

  async sendSongToDeckB(e) {
    e.stopPropagation();
    const shouldLoad = await this.maybeEnsureMusicVideo(this.song, this.mvRestrict);
    if (!shouldLoad) return;
    const [filepath, iconpath] = await this.getFilepathAndIconpath(this.song, this.useStems, this.mvRestrict);
    if (this.settings.getBool('virtualdj.active')) {
      window.vdj.send(`deck right load "${filepath}"`);
      this.selectSong(e);
    } else {
      this.dragSongTo(this.settings.getString('dragAndDrop.deckRightX'), this.settings.getString('dragAndDrop.deckRightY'));
      setTimeout(() => this.selectSong(e), 1000);  // wait to select song until after drag event
    }
  }

  dragSongTo(x, y) {
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
      iconButtonStyles,
      starRatingStyles,
      css`
        :host([selected]) .song-item {
          background-color: var(--ss-song-list-item-hover-background-color);
        }
        .star-rating {
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
          flex-shrink: 0;
        }
        .song-item:hover {
          background-color: var(--ss-song-list-item-hover-background-color);
        }
        icon-button, .icon-button {
          display: block;
          --mdc-icon-size: 16px;
          padding: 0 3px;
          visibility: hidden;
        }
        .song-item:hover > icon-button,
        .song-item:hover > .icon-button {
          visibility: visible;
        }
        .song-item > .edit {
          visibility: visible;
          display: none;
        }
        .song-item:hover > .edit {
          display: block;
        }
        .song-item .artist {
          color: var(--ss-song-list-item-artist-color);
        }
        .song-item .title {
          color: var(--ss-song-list-item-title-color);
        }
        .song-item .comments {
          color: var(--ss-song-list-item-comments);
          display: inline-block;
        }
        .song-item .warn {
          color: var(--ss-song-list-item-comments-warn);
        }
        .material-icons {
          font-family: 'Material Icons';
          font-weight: normal;
          font-style: normal;
          font-size: var(--mdc-icon-size, 24px);
          line-height: 1;
          letter-spacing: normal;
          text-transform: none;
          display: inline-block;
          white-space: nowrap;
          word-wrap: normal;
          direction: ltr;
          -webkit-font-smoothing: antialiased;
          text-rendering: optimizeLegibility;
          -moz-osx-font-smoothing: grayscale;
          font-feature-settings: 'liga';
        }
        .song-item .explicit {
          --mdc-icon-size: 16px;
          font-size: var(--mdc-icon-size, 16px);
          vertical-align: -3px;
          margin-right: 4px;
        }
        .song-item .mv-indicator {
          --mdc-icon-size: 18px;
          font-size: var(--mdc-icon-size, 18px);
          margin: 0 4px;
          flex-shrink: 0;
          display: inline-flex;
          align-items: center;
          justify-content: center;
          user-select: none;
        }
        .song-item .mv-indicator.has-video {
          color: var(--ss-song-list-item-mv-color, var(--accent2, #1EC8FA));
        }
        .song-item .mv-indicator.no-video {
          color: var(--ss-song-list-item-mv-off-color, var(--clr_disabled, #666));
          opacity: 0.6;
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
          background-image: var(--ss-song-list-item-waveform-background-image);
          filter: var(--ss-song-list-item-waveform-color-filter);
          height: 24px;
          width: 200px;
          padding: 0 3px;
        }

        .song-item .key {
          position: relative;
          overflow: hidden;
          white-space: nowrap;
          flex: 1 1 auto;
          min-width: 0;
          flex-shrink: 1;
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
          min-width: 64px;
          width: 64px;
        }
      `,
    ];
  }
}

window.customElements.define('song-list-item', SongListItem);
