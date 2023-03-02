import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-button";
import "@polymer/paper-checkbox/paper-checkbox.js";
import "@polymer/paper-input/paper-input.js";

import { AlertsMixin } from "./mixin-alerts.js";
import "./abstract-action-page.js";
import "./audio-player.js";
import "./genre-list.js";
import "./icon-button.js";
import "./options-menu-item.js";
import "./options-menu-paper-input.js";


class SongEdit extends AlertsMixin(LitElement) {
  render() {
    let basicGenreItems = this.basicGenreList.map(g => html`<options-menu-item>${g.name}</options-menu-item>`);
    let editedSongFilepath = this.soulSifterSettings.getString('music.dir') + this.path.dirname(this.editedSong.filepath);
    return html`
      <div class="mainContainer">
        <abstract-action-page id="abstractActionPage" @cancel="${this.skip}" @accept="${this.save}">
          <div class="fields">
            <div class="prev">&nbsp;<span>${this.taggedSong.artist}</span></div>
            <paper-input label="Artist" value="${this.editedSong.artist}" @change="${this.artistChanged}" required="true" floatingLabel id="artist"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.track}</span></div>
            <paper-input label="Track Num" value="${this.editedSong.track}" floatingLabel id="track"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.title}</span></div>
            <paper-input label="Title" value="${this.editedSong.title}" @change="${this.titleChanged}" required="true" floatingLabel id="title"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.remixer}</span></div>
            <paper-input label="Remixer" value="${this.editedSong.remixer}" floatingLabel id="remixer"></paper-input>
            <paper-input label="Comments" value="${this.editedSong.comments}" floatingLabel id="comments"></paper-input>
            <table>
              <tr>
                <td><paper-checkbox ?checked="${this.editedSong.lowQuality}" id="lowQuality">Low Quality</paper-checkbox></td>
                <td><paper-checkbox ?checked="${this.editedSong.album.mixed}" id="album_mixed">Mixed</paper-checkbox></td>
              </tr>
            </table>
          </div>
          <div class="fields">
            <paper-input label="Curator" value="${this.editedSong.curator}" floatingLabel id="curator"></paper-input>
            <paper-input label="YouTube ID" value="${this.editedSong.youtubeId}" floatingLabel id="youtube_id"></paper-input>
            <div id="ratingContainer">
              <div class="label">Rating</div>
              <star-rating value="${this.editedSong.rating}" id="rating"></star-rating>
            </div>
            <div class="prev">&nbsp;<span>${this.taggedSong.bpm}</span></div>
            <div class="input-row">
              <paper-input label="Key" value="${this.editedSong.tonicKey}" @value-changed="${this.tonicKeyInputChanged}" floatingLabel id="tonicKey"></paper-input>
              <icon-button icon="${this.editedSong.bpmLock ? "lock" : "lock_open"}" @click="${this.lockTonicKey}" id="tonic_key_lock"></icon-button>
            </div>
            <div class="input-row">
              <paper-input label="BPM" value="${this.editedSong.bpm}" @value-changed="${this.bpmInputChanged}" floatingLabel id="bpm"></paper-input>
              <icon-button icon="${this.editedSong.bpmLock ? "lock" : "lock_open"}" @click="${this.lockBpm}" id="bpm_lock"></icon-button>
            </div>
            <mwc-button @click="${this.rescanBpmAction}" id="bpmBtn">Rescan BPM</mwc-button>
            <table>
              <tr>
                <td><paper-checkbox ?checked="${this.editedSong.trashed}" ?disabled="${this.songIsTrashed}" id="trashed">Trash</paper-checkbox></td>
              </tr>
            </table>
          </div>
          <div class="fields">
            <div class="prev">&nbsp;<span>${this.taggedSong.album.artist}</span></div>
            <paper-input label="Album Artist" value="${this.editedSong.album.artist}" @value-changed="${this.albumArtistValChanged}" floatingLabel id="album_artist"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.name}</span></div>
            <paper-input label="Album Name" value="${this.editedSong.album.name}" required="true" floatingLabel id="album_name"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.albumPart.pos}</span></div>
            <paper-input label="POS" value="${this.editedSong.albumPart.pos}" floatingLabel id="albumpart_pos"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.albumPart.name}</span></div>
            <paper-input label="Subtitle" value="${this.editedSong.albumPart.name}" floatingLabel id="albumpart_name"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.label}</span></div>
            <paper-input label="Label" value="${this.editedSong.album.label}" floatingLabel id="album_label"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.catalogId}</span></div>
            <paper-input label="Catalog ID" value="${this.editedSong.album.catalogId}" floatingLabel id="album_catalogId"></paper-input>
          </div>
          <div class="fields">
            <div class="prev">&nbsp;<span>${this.taggedBasicGenreName}</span></div>
            <options-menu-paper-input label="Artist Genre" value="${this.basicGenreName}" required id="basicGenreInput">
              ${basicGenreItems}
            </options-menu-paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateYear}</span></div>
            <paper-input label="Release Year" value="${this.editedSong.album.releaseDateYear}" required="true" type="number" min="1600" max="2099" floatingLabel id="album_releaseDateYear"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateMonth}</span></div>
            <paper-input label="Release Month" value="${this.editedSong.album.releaseDateMonth}" type="number" min="0" max="12" floatingLabel id="album_releaseDateMonth"></paper-input>
            <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateDay}</span></div>
            <paper-input label="Release Day" value="${this.editedSong.album.releaseDateDay}" type="number" min="0" max="31" floatingLabel id="album_releaseDateDay"></paper-input>
            <div id="cover" @click="${this.changeCoverFile}"></div>
            <input style="display:none;" id="coverFileDialog" type="file" nwworkingdir="${editedSongFilepath}" @change="${this.changeCoverFileAction}"/>
          </div>
          <div class="genres">
            <genre-list id="genreList" .genres="${this.genres}" singleselect></genre-list>
          </div>
        </abstract-action-page>
        <div class="metadata">
          <audio-player id="audio" .song="${this.song}"></audio-player>
          <div>${this.editedSong.id}</div>
          <div @click="${this.changeSongFile}">${this.editedSong.filepath}</div>
          <input style="display:none;" id="songFileDialog" type="file" nwworkingdir="${editedSongFilepath}" @change="${this.changeSongFileAction}"/>
        </div>
      </div>
    `;
  }

  static get properties() {
    return {
      song: { type: Object },
      filepaths: { type: Array },
      taggedSong: { type: Object },
      editedSong: { type: Object },
      soulSifterSettings: { type: Object },
      newSongManager: { type: Object },
      albumArtistCompilation: { type: Boolean },
      changedAlbumCover: { type: Boolean },
      changedSongFile: { type: Boolean },
      genres: { type: Array },
      basicGenreList: { type: Array },
      basicGenreName: { type: String },
      taggedBasicGenreName: { type: String },
      songIsTrashed: { type: Boolean },
    }
  }

  constructor() {
    super();
    this._openPageListener = (e) => {
      this.videoUrl = undefined;
      if (e.detail.song) {
        this.song = e.detail.song;
        this.taggedSong = new ss.Song(this.song);
        this.sterilizeSong(this.taggedSong);
        this.filepaths = [];
        this.songChanged();
      } else if (e.detail.filepaths) {
        this.song = null;
        this.filepaths = e.detail.filepaths;
        this.videoUrl = e.detail.videoUrl;
        this.filepathsChanged();
      }
      this.classList.add('show');
      if (e.detail.autoplay) setTimeout(() => this.shadowRoot.getElementById('audio').play(), 1);
    };
    // Defaults
    this.songIsTrashed = false;
    this.taggedBasicGenreName = '';
    this.filepaths = [];
    this.soulSifterSettings = new ss.SoulSifterSettings();
    this.newSongManager = new ss.NewSongManager();
    this.albumArtistCompilation = false;
    this.changedAlbumCover = false;
    this.changedSongFile = false;
    this.genres = [];
    this.basicGenreList = ss.BasicGenre.findAll();
    // used for moving cover files
    this.fs = require('fs');
    this.path = require('path');
    // Dummy values for rendering
    this.taggedSong = new ss.Song();
    this.sterilizeSong(this.taggedSong);
    this.editedSong = new ss.Song();
    this.sterilizeSong(this.editedSong);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('song-edit', this._openPageListener);
  }

  disconnectedCallback() {
    window.removeEventListener('song-edit', this._openPageListener);
    super.disconnectedCallback();
  }

  exit() {
    this.videoUrl = undefined;
    this.sterilizeSong(this.editedSong);
    this.shadowRoot.getElementById('audio').pause();
    this.classList.remove('show');
    let event = new CustomEvent('song-edit-close', { detail: this.savedPreviously });
    window.dispatchEvent(event);
  }

  save() {
    if (!this.validate()) return;
    this.syncEdits();
    // If there is no album part, we should clear this object out so it is
    // not saved. Setting the ID deletes the object, and having an ID of 0
    // indicates a NULL value upon save.
    if (!this.editedSong.albumPart.pos && !this.editedSong.albumPart.name) {
      this.editedSong.albumPartId = 0;
    } else {
      // make sure this albumPart's album matches this edited album
      if (!!this.editedSong.album.id) this.editedSong.albumPart.albumId = this.editedSong.album.id;
      else this.editedSong.albumPart.album = this.editedSong.album;
    }

    // Moves files after song change
    // TODO this whole section should move to MusicService
    if (this.changedSongFile && !!this.editedSong.id) {
      // remove current song (copied to /var/tmp)
      let currentSongFilepath = this.soulSifterSettings.getString('music.dir') + this.song.filepath;
      let albumFilepath = this.path.dirname(this.song.filepath);
      let rmToTmpFilename = '/var/tmp/' + this.path.basename(this.song.filepath);
      try {
        this.fs.renameSync(currentSongFilepath, rmToTmpFilename);
        console.log('Renamed ' + currentSongFilepath + ' to ' + rmToTmpFilename);
      } catch(err) {
        console.warn('Unable to remove file because I probably cannot find it.');
        console.warn(err);
      }
      // copy new file to current albumFilepaths location
      let basename = ss.MusicService.cleanDirName(this.path.basename(this.editedSong.filepath));
      let newpath = this.soulSifterSettings.getString('music.dir') + albumFilepath + '/' + basename;
      if (newpath != this.editedSong.filepath) {
        this.fs.renameSync(this.editedSong.filepath, newpath);
        console.log('Renamed ' + this.editedSong.filepath + ' to ' + newpath);
      }
      this.editedSong.filepath = albumFilepath + '/' + basename;
    }

    if (this.editedSong.id) {
      this.editedSong.update();
    } else {
      this.newSongManager.processSong(this.editedSong);
      if (!!this.videoUrl) {
        ss.MusicVideoService.associateYouTubeVideo(this.editedSong, this.videoUrl);
        this.videoUrl = undefined;
      }
      this.albumArtistCompilation = !this.editedSong.album.albumArtist;
      if (this.hasMoreSongs()) {
        this.shadowRoot.getElementById('audio').pause();
        return;  // stay on this page
      }
      this.albumArtistCompilation = false;
    }

    // Currently you cannot edit the cover of newly added songs. The
    // NewSongManager processes it last, so anything changed will just be
    // overwritten.
    if (this.changedAlbumCover) {
      // TODO the file moving should be done externally
      let basename = ss.MusicService.cleanDirName(this.path.basename(this.editedSong.album.coverFilepath));
      let dirname = this.path.dirname(this.editedSong.filepath);
      let newpath = this.soulSifterSettings.getString('music.dir') + dirname + '/' + basename;
      // TODO use rename with callback
      if (this.editedSong.album.coverFilepath != newpath) this.fs.renameSync(this.editedSong.album.coverFilepath, newpath);
      this.editedSong.album.coverFilepath = dirname + '/' + basename;
      this.editedSong.album.update();
    }

    this.savedPreviously = true;
    this.exit();
  }

  skip() {
    if (this.hasMoreSongs()) {
      return;  // stay on this page
    }
    this.albumArtistCompilation = false;
    this.savedPreviously = false;
    this.exit();
  }

  songChanged() {
    if (!this.song) return;
    this.songIsTrashed = this.song.trashed;
    this.editedSong = new ss.Song(this.song);
    this.sterilizeSong(this.song);
    this.sterilizeSong(this.editedSong);
    if (!!this.editedSong.album.basicGenre) this.basicGenreName = this.editedSong.album.basicGenre.name;
    else this.basicGenreName = '';
    this.shadowRoot.getElementById('cover').style.backgroundImage = 'url("file://' + this.soulSifterSettings.getString('music.dir') + this.editedSong.album.coverFilepath + '")';
    this.changedAlbumCover = false;
    this.changedSongFile = false;
    // this does not seem to play well when working straight on the array object
    this.genres = this.editedSong.styles;
    this.forceEdits();
  }

  filepathsChanged() {
    this.newSongManager.import(this.filepaths);
    if (!this.hasMoreSongs()) {
      // for some reason there is no song after importing filepaths, so just exit.
      this.exit();
    }
  }

  sterilizeSong(s) {
    // Create album & album part for the song we are editing if there
    // currently does not exist one.
    if (!s.album) s.album = new ss.Album();
    if (!s.albumPart) {
      s.albumPart = new ss.AlbumPart();
      s.albumPart.album = s.album;
    }
  }

  hasMoreSongs() {
    // There is an issue to update the data bindings of the songs updated
    // from the nextSong call on NewSongManager. Because of the dirty
    // checking in notifyPath, and the internals of the object are updated
    // via a 3rd-party library, we cannot update the bindings.
    // There are two options:
    // 1. Create temporary song objects to get updated, then set the bound
    // objects to these values.
    // 2. Call _pathEffector which is the "private" method in polymer which
    // does the update calls of notifyPath without the dirty checking.
    // Going with option 1 for now. Even though it is creating extra objects,
    // it's relatively small, and we could make them private members ourself
    // if we care about memory that much. It should be better than being
    // bitten by using a method that is considered private and updated from
    // underneath us.
    let tmpSong = new ss.Song();
    let tmpTaggedSong = new ss.Song();
    if (this.newSongManager.nextSong(tmpSong, tmpTaggedSong)) {
      this.basicGenreName = '';
      this.taggedBasicGenreName = '';
      this.taggedSong = tmpTaggedSong;
      this.sterilizeSong(this.taggedSong);
      this.song = this.titleCaseSong(tmpSong);
      this.sterilizeSong(this.song);
      this.songChanged();
      return true;
    }
    this.sterilizeSong(this.taggedSong);
    this.sterilizeSong(this.song);
    this.sterilizeSong(this.editedSong);
    return false;
  }

  rescanBpmAction() {
    let bpms = ss.AudioAnalyzer.analyzeBpm(this.song);
    let tmp = [];
    for (let i = 0; i < 5 && i < bpms.length; ++i) {
      tmp[i] = Math.round(bpms[i]);
    }
    this.taggedSong.bpm = tmp.join(', ');
    this.requestUpdate();
  }

  validate() {
    let valid = true;
    let inputs = this.shadowRoot.querySelectorAll('paper-input');
    for (let i = 0; i < inputs.length; ++i) {
      valid &= inputs[i].validate();
      if (!inputs[i].validate()) this.addAlert(inputs[i].label + " input is invalid.");
    }
    if (this.shadowRoot.getElementById('basicGenreInput').value == '') {
      valid = false;
      this.addAlert("BasicGenre input is invalid.");
    }
    return valid;
  }

  albumArtistValChanged(e) {
    let artist = this.shadowRoot.getElementById('album_artist').value;
    if (!!artist && !this.taggedBasicGenreName) {
      let basicGenre = ss.BasicGenre.findByArtist(artist);
      if (!!basicGenre) {
        this.basicGenreName = basicGenre.name;
        this.shadowRoot.getElementById('basicGenreInput').value = this.basicGenreName;
        this.taggedBasicGenreName = this.basicGenreName;
      } else {
        this.taggedBasicGenreName = '';
      }
    }
  }

  artistChanged(e) {
    let artist = this.shadowRoot.getElementById('artist').value;
    if (!!artist &&
        ((!!this.taggedSong.album.artist && this.taggedSong.artist.toUpperCase() == this.taggedSong.album.artist.toUpperCase()) ||
         (!this.taggedSong.album.artist && this.taggedSong.artist.toUpperCase() == this.song.album.artist.toUpperCase()))) {
      artist = artist.replace(/\s*\(.*\)$/g, '');
      this.shadowRoot.getElementById('album_artist').value = artist;
    }
  }

  titleChanged(e) {
    let title = this.shadowRoot.getElementById('title').value;
    if (!!title &&
        ((!!this.taggedSong.album.name && this.taggedSong.title.toUpperCase() == this.taggedSong.album.name.toUpperCase()) ||
         (!this.taggedSong.album.name && this.taggedSong.title.toUpperCase() == this.song.album.name.toUpperCase()))) {
      this.shadowRoot.getElementById('album_name').value = title;
    }
  }

  changeCoverFile() {
    this.shadowRoot.getElementById('coverFileDialog').click();
  }

  changeCoverFileAction(e) {
    let val = e.currentTarget.value;
    this.shadowRoot.getElementById('cover').style.backgroundImage = 'url("file://' + val + '")';
    this.editedSong.album.coverFilepath = val;
    this.changedAlbumCover = true;
  }

  changeSongFile() {
    this.shadowRoot.getElementById('songFileDialog').click();
  }

  changeSongFileAction(e) {
    let val = e.currentTarget.value;
    this.editedSong.filepath = val;
    this.changedSongFile = true;
  }

  lockBpm() {
    this.editedSong.bpmLock = !this.editedSong.bpmLock;
    this.shadowRoot.getElementById('bpm_lock').icon = this.editedSong.bpmLock ? "lock" : "lock_open";
  }

  bpmInputChanged() {
    let bpm = this.shadowRoot.getElementById('bpm').value;
    if (!!bpm && !!this.song && !!this.song.id && bpm != this.song.bpm && !this.editedSong.bpmLock) {
      this.lockBpm();
    }
  }

  lockTonicKey() {
    this.editedSong.tonicKeyLock = !this.editedSong.tonicKeyLock;
    this.shadowRoot.getElementById('tonic_key_lock').icon = this.editedSong.tonicKeyLock ? "lock" : "lock_open";
  }

  tonicKeyInputChanged() {
    let key = this.shadowRoot.getElementById('tonicKey').value;
    if (!!key && !!this.song && !!this.song.id && key != this.song.tonicKey && !this.editedSong.tonicKeyLock) {
      this.lockTonicKey();
    }
  }

  titleCaseSong(s) {
    s.artist = this.titleCase(s.artist);
    s.title = this.titleCase(s.title);
    s.remixer = this.titleCase(s.remixer);
    if (s.album) {
      s.album.artist = this.titleCase(s.album.artist);
      s.album.name = this.titleCase(s.album.name)
    }
    return s;
  }

  titleCase(str) {
    return str.replace(/\w\S*/g, txt => {
      if (/^\(?(ft|feat|rmx|flip|reflip|mix|remix|reboot)\)?$/.test(txt.toLowerCase())) return txt.toLowerCase();
      let numChars = 0;
      let numUppers = 0;
      for(let i = 0; i < txt.length; i++) {
        if (/[A-Z]/.test(txt.charAt(i))) numUppers++;
        if (/[a-zA-Z]/.test(txt.charAt(i))) numChars++;
      }
      if (numChars <= 3 || numUppers <= 2) return txt;
      else return txt.charAt(0).toUpperCase() + txt.substr(1).toLowerCase()
    });
  }

  forceEdits() {
    // stupid this is required
    this.shadowRoot.getElementById('artist').value = this.song.artist.trim();
    this.shadowRoot.getElementById('track').value = this.song.track.trim();
    this.shadowRoot.getElementById('title').value = this.song.title.trim();
    this.shadowRoot.getElementById('remixer').value = this.song.remixer.trim();
    this.shadowRoot.getElementById('comments').value = this.song.comments.trim();
    this.shadowRoot.getElementById('curator').value = this.song.curator.trim();
    this.shadowRoot.getElementById('youtube_id').value = this.song.youtubeId.trim();
    this.shadowRoot.getElementById('rating').value = this.song.rating;
    this.shadowRoot.getElementById('bpm').value = this.song.bpm.trim();
    this.shadowRoot.getElementById('bpm_lock').icon = this.song.bpmLock ? "lock" : "lock_open";
    this.shadowRoot.getElementById('tonicKey').value = this.song.tonicKey.trim();
    this.shadowRoot.getElementById('tonic_key_lock').icon = this.song.tonicKeyLock ? "lock" : "lock_open";
    this.shadowRoot.getElementById('lowQuality').checked = this.song.lowQuality;
    this.shadowRoot.getElementById('album_mixed').checked = this.song.album.mixed;
    this.shadowRoot.getElementById('trashed').checked = this.song.trashed;
    this.shadowRoot.getElementById('album_artist').value = this.song.album.artist.trim();
    this.albumArtistValChanged();  // I'm not sure why this isn't always called from the above change.
    this.shadowRoot.getElementById('album_name').value = this.song.album.name.trim();
    this.shadowRoot.getElementById('albumpart_pos').value = this.song.albumPart.pos;
    this.shadowRoot.getElementById('albumpart_name').value = this.song.albumPart.name.trim();
    this.shadowRoot.getElementById('album_label').value = this.song.album.label.trim();
    this.shadowRoot.getElementById('album_catalogId').value = this.song.album.catalogId.trim();
    this.shadowRoot.getElementById('album_releaseDateYear').value = this.song.album.releaseDateYear;
    this.shadowRoot.getElementById('album_releaseDateMonth').value = this.song.album.releaseDateMonth;
    this.shadowRoot.getElementById('album_releaseDateDay').value = this.song.album.releaseDateDay;
    // my styles component works w/o this crap
  }

  syncEdits() {
    let song = this.editedSong;
    song.artist = this.shadowRoot.getElementById('artist').value;
    song.track = this.shadowRoot.getElementById('track').value;
    song.title = this.shadowRoot.getElementById('title').value;
    song.remixer = this.shadowRoot.getElementById('remixer').value;
    song.comments = this.shadowRoot.getElementById('comments').value;
    song.curator = this.shadowRoot.getElementById('curator').value;
    song.youtubeId = this.shadowRoot.getElementById('youtube_id').value;
    song.rating = this.shadowRoot.getElementById('rating').value;
    song.bpm = this.shadowRoot.getElementById('bpm').value;
    song.tonicKey = this.shadowRoot.getElementById('tonicKey').value;
    song.lowQuality = this.shadowRoot.getElementById('lowQuality').checked;
    song.album.mixed = this.shadowRoot.getElementById('album_mixed').checked;
    song.trashed = this.shadowRoot.getElementById('trashed').checked;
    // TODO song.album.cover = this.shadowRoot.getElementById('album_cover').value;
    song.album.artist = this.shadowRoot.getElementById('album_artist').value;
    song.album.name = this.shadowRoot.getElementById('album_name').value;
    song.albumPart.pos = this.shadowRoot.getElementById('albumpart_pos').value;
    song.albumPart.name = this.shadowRoot.getElementById('albumpart_name').value;
    song.album.label = this.shadowRoot.getElementById('album_label').value;
    song.album.catalogId = this.shadowRoot.getElementById('album_catalogId').value;
    song.album.releaseDateYear = this.shadowRoot.getElementById('album_releaseDateYear').value;
    song.album.releaseDateMonth = this.shadowRoot.getElementById('album_releaseDateMonth').value;
    song.album.releaseDateDay = this.shadowRoot.getElementById('album_releaseDateDay').value;
    // I think the reason you can use the genres is it's an array of items that maps back
    let styleIds = this.genres.map(function(g) { return g.id; });
    this.editedSong.styleIds = styleIds;
    // Since basicGenre objects should not be shared between objects, we will be
    // creating another basicGenre here regardless if we selected an object in the dropdown.
    this.basicGenreName = this.shadowRoot.getElementById('basicGenreInput').value;
    this.editedSong.album.basicGenre = ss.BasicGenre.findByName(this.basicGenreName);
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
        .mainContainer {
          display: relative;
          height: 100%;
          width: 100%;
        }
        .fields {
          display: flex;
          flex-direction: column;
          justify-content: center;
          margin: 10px;
        }
        .genres {
          height: 372px;
          width: 240px;
          margin: 10px;
        }
        #bpmBtn {
          align-self: flex-end;
          margin-top: -10px;
          margin-right: -5px;
          margin-bottom: -26px;
        }

        paper-input {
          position: relative;
          width: 240px;
        }
        paper-input::shadow .label-is-floating label {
          text-align: right;
        }
        paper-dropdown-menu::shadow paper-input::shadow label {
          text-align: right;
        }

        paper-checkbox {
          margin: 10px 0;
        }

        #ratingContainer {
          position: relative;
          width: 240px;
          height: 62px;
        }
        #ratingContainer .label {
          position: absolute;
          top: 11px;
        }
        #ratingContainer #rating {
          position: absolute;
          top: 30px;
        }
        .label {
          font-size: 0.75em;
        }
        .prev {
          font-size: 13px;
          margin-top: 8px;
          margin-bottom: -23px;
          text-align: right;
          color: #666;  /* TODO pull into theme */
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
          background-color: var(--primary-background-color);
        }
        .metadata div {
          padding: 0 20px;
          white-space: nowrap;
        }
        audio-player {
          height: 24px;
          width: 100%;
          flex-grow: 10;
        }

        .cols {
          display: flex;
          flex-direction: row;
          justify-content: space-between;
        }
        .rows {
          display: flex;
          flex-direction: column;
        }
        #cover {
          align-self: center;
          background-position: center center;
          background-size: cover;
          background-color: #333;  /* TODO put into theme */
          height: 124px;
          width: 124px;
        }

        .input-row {
          display: flex;
          align-items: flex-end;
          width: 240px;
        }
        icon-button {
          margin-bottom: 6px;
        }
      `,
    ];
  }
}

window.customElements.define('song-edit', SongEdit);
