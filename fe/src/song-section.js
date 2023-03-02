import { css, html, LitElement } from "@polymer/lit-element";

import "@polymer/paper-input/paper-input.js";

import "./audio-player.js";
import "./icon-button.js";
import "./pitch-slider.js";
import "./star-rating.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongMixin } from "./mixin-song.js";
import { SongTrailMixin } from "./mixin-song-trail.js";


class SongSection extends SettingsMixin(SongMixin(SongTrailMixin(LitElement))) {
  render() {
    let localeDateTime = !!this.song ? new Date(this.song.dateAdded).toLocaleString() : '';
    return html`
      <div id="nav" draggable="true" @dragstart="${this.dragSong}">
        <icon-button @click="${this.backAction}" ?disabled="${!this.songTrail.length}" icon="arrow_back"></icon-button>
        <icon-button @click="${this.forwardAction}" ?disabled="${!this.songTrailCache.length}" icon="arrow_forward"></icon-button>
      </div>
      <div class="content">
        <div class="row">
          <div class="title">${this.song.title}</div>
          <icon-button icon="edit" class="edit-icon self-end" @click="${this.openEditSongPage}"></icon-button>
        </div>
        <div class="row">
          <div class="artist">${this.song.artist}</div>
          <icon-button icon="album" class="edit-icon self-end" @click="${this.openEditMixPage}" ?disabled="${this.songTrail.length < 2}"></icon-button>
        </div>
        <star-rating value="${this.song.rating}" readonly></star-rating>
        <div class="track self-end">${this.song.album.track}</div>
        <div class="track self-end">${this.song.album.name}</div>
        <div class="released self-end"><span>${this.song.album.releaseDateYear}</span>-<span>${this.song.album.releaseDateMonth}</span>-<span>${this.song.album.releaseDateDay}</span></div>
        <div class="comments">${this.song.comments}</div>
        <div class="bpm">${this.song.bpm} bpm</div>
        <time class="added">${localeDateTime}</time>
        <div class="slider">
          <div>pitch</div>
          <pitch-slider></pitch-slider>
        </div>
        <audio-player id="audio" .song="${this.song}"></audio-player>
        <div>${this.song.styles.map(s => s.name).join(', ')}</div>
        <div id="musicVideoThumbnail" draggable="true" @dragstart="${this.dragMusicVideo}" ?hide="${!this.musicVideo}"></div>
        <div id="musicVideoInput" ?hide="${!!this.musicVideo}">
          <paper-input label="video url" no-label-float id="videoUrlInput"></paper-input>
          <icon-button @click="${this.associateVideo}" icon="movie"></paper-icon-button>
        </div>
      </div>
    `;
  }

  static get properties() {
    return {
      songTrailCache: { type: Array },
      // do not clear the cache when the songtrail is changed using the fwd or back btns
      clearCache: { type: Boolean },
      musicVideo: { type: Object },
    }
  }

  constructor() {
    super();
    this.songTrail = [];
    this.songTrailCache = [];
    this.clearCache = true;
    this.saveSongTrailListener = (e) => this.saveSongTrail(e);
    this.song = new ss.Song();
    this.song.album = new ss.Album();
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('save-song-trail', this.saveSongTrailListener);
  }

  disconnectedCallback() {
    window.removeEventListener('save-song-trail', this.saveSongTrailListener);
    super.disconnectedCallback();
  }

  songChanged(song) {
    this.song = song;
    // TODO this.$.audio.pause();
    if (!song) {
      this.song = new ss.Song();
      this.song.album = new ss.Album();
      this.setCoverImage();
      this.setMusicVideo();
      return;
    }
    let entry = new ss.PlaylistEntry();
    entry.songId = song.id;
    entry.position = this.songTrail.length;
    this.songTrail.push(entry);
    this.changeSongTrail(this.songTrail);
    if (this.clearCache) this.songTrailCache = [];
    this.clearCache = true;
    this.shadowRoot.getElementById('videoUrlInput').value = '';
    this.setCoverImage(this.settings.getString('music.dir') + this.song.album.coverFilepath);
    this.setMusicVideo(this.song.musicVideo);
  }

  dragSong(e) {
    e.dataTransfer.setData('text/plain', 'extensis-filenames-type:' + this.settings.getString('music.dir') + this.song.filepath);
  }

  backAction(e) {
    let entry = this.songTrail.pop();
    if (!!entry) {
      this.songTrailCache.push(entry);
    }
    this.clearCache = false;
    if (this.songTrail.length) {
      // This gets added back to the song trail with songChanged method above. However, since this object gets destroyed
      // we need to make sure the song doesn't get destroyed with it.
      this.changeSong(ss.Song.findById(this.songTrail.pop().songId));
    } else {
      this.changeSong(undefined);
    }
    this.changeSongTrail(this.songTrail);
  }

  forwardAction(e) {
    this.clearCache = false;
    this.changeSong(ss.Song.findById(this.songTrailCache.pop().songId));
  }

  saveSongTrail(e) {
    let playlist = e.detail;
    for (let i = 0; i < this.songTrail.length; ++i) {
      let entry = this.songTrail[i];
      entry.playlist = playlist;
      entry.save();
    }
  }

  setCoverImage(img) {
    let url = '';
    if (!!img) {
      url = 'url("file://' + img + '")';
    }
    this.shadowRoot.getElementById('nav').style.backgroundImage = url;
  }

  setMusicVideo(mv) {
    this.musicVideo = mv;
    var url = '';
    if (!!mv && !!mv.thumbnailFilePath) {
      url = 'url("file://' + this.settings.getString('mv.dir') + mv.thumbnailFilePath + '")';
    }
    this.shadowRoot.getElementById('musicVideoThumbnail').style.backgroundImage = url;
  }

  associateVideo() {
    if (!this.song) return;
    let videoUrl = this.shadowRoot.getElementById('videoUrlInput').value;
    this.setMusicVideo(ss.MusicVideoService.associateYouTubeVideo(this.song, videoUrl));
  }

  dragMusicVideo(e) {
    e.dataTransfer.setData('text/plain', 'extensis-filenames-type:' + this.settings.getString('mv.dir') + this.musicVideo.filePath);
  }

  openEditSongPage(e) {
    let event = new CustomEvent('song-edit', { detail: { song: this.song } });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  openEditMixPage(e) {
    let len = this.songTrail.length;
    if (len < 2) {
      console.warn('Cannot create a mix without more songs.');
      return;
    }
    var mix = new ss.Mix();
    mix.outSongId = this.songTrail[len - 2].songId;
    mix.inSongId = this.songTrail[len - 1].songId;
    let event = new CustomEvent('mix-edit', { detail: mix });
    window.dispatchEvent(event);
    e.stopPropagation();
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
        #nav {
          height: 256px;
          display: flex;
          flex-direction: row;
          justify-content: space-between;
          background-color: transparent;
          background-size: cover;
          background-position: center;
        }
        icon-button[disabled] {
          visibility: hidden;
        }
        icon-button {
          --mdc-icon-size: 24px;
          color: var(--primary-text-color);
        }
        .hide {
          display: none;
        }

        
        .content {
          margin: 5px 15px 10px 15px;
          display: flex;
          flex-direction: column;
        }
        .title {
          color: var(--secondary-text-color);
        }
        .row {
          display: flex;
          flex-direction: row;
          justify-content: space-between;
          align-items: flex-end;
        }
        options-menu {
          margin-right: -10px;
        }
        .slider {
          display: flex;
          flex-direction: row;
          align-items: center;
        }
        pitch-slider {
          margin-left: 16px;
        }
        .self-end {
          align-self: flex-end;
        }
        .edit-icon {
          --mdc-icon-size: 16px;
        }
        audio-player {
          height: 30px;
          width: 100%;
        }
        #musicVideoInput {
          display: flex;
          flex-direction: row;
        }
        #musicVideoInput[hide] {
          display: none;
        }
        #musicVideoThumbnail {
          width: 100%;
          height: 144px;
          background-size: cover;
        }
        #musicVideoThumbnail[hide] {
          display: none;
        }
      `
    ];
  }
}

window.customElements.define('song-section', SongSection);
