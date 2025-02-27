import { css, html, LitElement } from "lit";

import "@material/web/button/text-button.js";
import "@material/web/checkbox/checkbox.js";
import "@material/web/textfield/filled-text-field.js";

import { AlertsMixin } from "./mixin-alerts-pub.js";
import { SettingsMixin } from "./mixin-settings.js";
import { SongEditMixin } from "./mixin-song-edit.js";
import "./abstract-action-page.js";
import "./audio-player.js";
import "./genre-list.js";
import "./icon-button.js";
import "./options-menu-item.js";
import "./options-menu-input.js";


class SongEdit extends AlertsMixin(SettingsMixin(SongEditMixin(LitElement))) {
  render() {
    let youtubeIdPattern = String.raw`[^"&?\/\s]{11}`;
    let basicGenreItems = this.basicGenreList.map(g => html`<options-menu-item>${g.name}</options-menu-item>`);
    return html`
      <div class="mainContainer">
        <abstract-action-page id="abstractActionPage" @cancel="${this.skip}" @accept="${this.save}">
          <div class="fields">
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.artist}</span></div>
              <md-filled-text-field label="Artist" .value="${this.editedSong.artist}" @change="${this.artistChanged}" required no-asterisk id="artist"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.track}</span></div>
              <md-filled-text-field label="Track Num" .value="${this.editedSong.track}" id="track"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.title}</span></div>
              <md-filled-text-field label="Title" .value="${this.editedSong.title}" @change="${this.titleChanged}" required no-asterisk id="title"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.remixer}</span></div>
              <md-filled-text-field label="Remixer" .value="${this.editedSong.remixer}" id="remixer"></md-filled-text-field>
            </div>
            <div>
              <md-filled-text-field label="Comments" .value="${this.editedSong.comments}" id="comments"></md-filled-text-field>
            </div>
            <div class="row">
              <label><md-checkbox ?checked="${this.editedSong.lowQuality}" id="lowQuality"></md-checkbox>Low Quality</label>
              <label><md-checkbox ?checked="${this.editedSong.album.mixed}" id="album_mixed"></md-checkbox>Mixed</label>
            </div>
          </div>
          <div class="fields">
            <div>
              <md-filled-text-field label="Curator" .value="${this.editedSong.curator}" id="curator"></md-filled-text-field>
            </div>
            <div>
              <md-filled-text-field label="YouTube ID" .value="${this.editedSong.youtubeId}" id="youtube_id" pattern="${youtubeIdPattern}"></md-filled-text-field>
            </div>
            <div>
              <div id="ratingContainer">
                <div class="label">Rating</div>
                <star-rating value="${this.editedSong.rating}" id="rating"></star-rating>
              </div>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.bpm}</span></div>
              <div class="input-row">
                <md-filled-text-field label="Key" .value="${this.editedSong.tonicKey}" @change="${this.tonicKeyInputChanged}" id="tonicKey" pattern="[A-G]b?m?"></md-filled-text-field>
                <icon-button icon="${this.editedSong.bpmLock ? "lock" : "lock_open"}" @click="${this.lockTonicKey}" id="tonic_key_lock"></icon-button>
              </div>
            </div>
            <div>
              <div class="input-row">
                <md-filled-text-field label="BPM" .value="${this.editedSong.bpm}" @change="${this.bpmInputChanged}" id="bpm" pattern="[0-9]*(\\.[0-9]*)?"></md-filled-text-field>
                <icon-button icon="${this.editedSong.bpmLock ? "lock" : "lock_open"}" @click="${this.lockBpm}" id="bpm_lock"></icon-button>
              </div>
              <md-text-button @click="${this.rescanBpmAction}" id="bpmBtn">Rescan BPM</md-text-button>
            </div>
            <div class="row">
              <label><md-checkbox ?checked="${this.editedSong.trashed}" ?disabled="${this.songIsTrashed}" id="trashed"></md-checkbox>Trash</label>
            </div>
          </div>
          <div class="fields">
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.artist}</span></div>
              <md-filled-text-field label="Album Artist" .value="${this.editedSong.album.artist}" @change="${this.albumArtistValChanged}" id="album_artist"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.name}</span></div>
              <md-filled-text-field label="Album Name" .value="${this.editedSong.album.name}" required no-asterisk id="album_name"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.albumPart.pos}</span></div>
              <md-filled-text-field label="POS" .value="${this.editedSong.albumPart.pos}" id="albumpart_pos"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.albumPart.name}</span></div>
              <md-filled-text-field label="Subtitle" .value="${this.editedSong.albumPart.name}" id="albumpart_name"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.label}</span></div>
              <md-filled-text-field label="Label" .value="${this.editedSong.album.label}" id="album_label"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.catalogId}</span></div>
              <md-filled-text-field label="Catalog ID" .value="${this.editedSong.album.catalogId}" id="album_catalogId"></md-filled-text-field>
            </div>
          </div>
          <div class="fields">
            <div>
              <div class="prev">&nbsp;<span>${this.taggedBasicGenreName}</span></div>
              <options-menu-input label="Artist Genre" value="${this.basicGenreName}" required no-asterisk id="basicGenreInput">
                ${basicGenreItems}
              </options-menu-input>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateYear}</span></div>
              <md-filled-text-field label="Release Year" .value="${this.editedSong.album.releaseDateYear}" required no-asterisk type="number" min="1600" max="2099" id="album_releaseDateYear"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateMonth}</span></div>
              <md-filled-text-field label="Release Month" .value="${this.editedSong.album.releaseDateMonth}" type="number" min="0" max="12" id="album_releaseDateMonth"></md-filled-text-field>
            </div>
            <div>
              <div class="prev">&nbsp;<span>${this.taggedSong.album.releaseDateDay}</span></div>
              <md-filled-text-field label="Release Day" .value="${this.editedSong.album.releaseDateDay}" type="number" min="0" max="31" id="album_releaseDateDay"></md-filled-text-field>
            </div>
            <div>
              <div id="cover" @click="${this.changeCoverFile}"></div>
            </div>
          </div>
          <div class="genres">
            <genre-list id="genreList" .genres="${this.genres}" singleselect></genre-list>
          </div>
        </abstract-action-page>
        <div class="metadata">
          <audio-player id="audio" .song="${this.song}"></audio-player>
          <icon-button @click=${this.prevSong} icon="chevron_left"></icon-button>
          <icon-button @click=${this.nextSong} icon="chevron_right"></icon-button>
          <div style="user-select:text">${this.editedSong.id}</div>
          <div style="user-select:text">(${this.editedSong.albumId})</div>
          <div @click="${this.changeSongFile}" style="user-select:text">${this.editedSong.filepath}</div>
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
        this.song = ss.Song.findById(e.detail.song.id);
        this.taggedSong = new ss.Song(this.song);
        this.sterilizeSong(this.taggedSong);
        this.filepaths = [];
        this.songChanged();
        this.classList.add('show');
      } else if (e.detail.filepaths) {
        this.song = null;
        this.filepaths = e.detail.filepaths;
        this.videoUrl = e.detail.videoUrl;
        this.filepathsChanged();
        if (!this.settings.getBool('edit.autoAdd')) this.classList.add('show');
      }
    };
    // Defaults
    this.songIsTrashed = false;
    this.taggedBasicGenreName = '';
    this.filepaths = [];
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

  checkIfSongExists() {
    // Check if this song already exists
    if (!this.editedSong.id) {
      try {
        let query = 'a:="' + this.shadowRoot.getElementById('artist').value + '" t:="' + this.shadowRoot.getElementById('title').value + '"';
        let songs = ss.SearchUtil.searchSongs(query, 1, 0, "", [], [], [], 0, false, 0, 0, (msg) => { this.addAlert(msg, 5); });
        if (songs.length > 0) {
          this.addAlert('Track with same artist & title already exists. (' + songs[0].artist +  ' - ' + songs[0].title + ')');
          // TODO get confirmation before adding
        }
      } catch(e) {
        console.error(e);
      }
    }
  }

  save() {
    // TODO throw exception
    if (!this.validate()) return;
    this.checkIfSongExists();
    // Check if this song should move the album folder
    let moveAlbum = false;
    if (this.editedSong.id) {
      // TODO support album parts
      if (this.song.album.basicGenre.name != this.shadowRoot.getElementById('basicGenreInput').value ||
          this.song.album.name != this.shadowRoot.getElementById('album_name').value ||
          this.song.album.artist != this.shadowRoot.getElementById('album_artist').value) {
        moveAlbum = true;
      }
    }
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
      let currentSongFilepath = this.settings.getString('dir.music') + this.song.filepath;
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
      let newpath = this.settings.getString('dir.music') + albumFilepath + '/' + basename;
      if (newpath != this.editedSong.filepath) {
        this.fs.renameSync(this.editedSong.filepath, newpath);
        console.log('Renamed ' + this.editedSong.filepath + ' to ' + newpath);
      }
      this.editedSong.filepath = albumFilepath + '/' + basename;
    }

    if (this.editedSong.id) {
      this.editedSong.update();
      this.notifySongEdited(this.editedSong.id);
      // ss.TagService.writeId3v2Tag(this.editedSong);
      if (!this.songIsTrashed && this.editedSong.trashed) this.newSongManager.trashMusicFile(this.editedSong);
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
      ss.MusicService.updateAlbumCover(this.editedSong.album.coverFilepath, this.editedSong.album,  (msg) => { this.addAlert(msg); });
    }

    if (moveAlbum) ss.MusicService.moveAlbum(this.editedSong.album,  (msg) => { this.addAlert(msg); });

    this.savedPreviously = true;
    this.exit();
  }

  skip() {
    if (this.hasMoreSongs()) {
      this.shadowRoot.getElementById('audio').pause();
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
    // this.sterilizeSong(this.song);  This affects original song, so do not want to change it.
    this.sterilizeSong(this.editedSong);
    if (!!this.editedSong.album.basicGenre) this.basicGenreName = this.editedSong.album.basicGenre.name;
    else this.basicGenreName = '';
    this.shadowRoot.getElementById('cover').style.backgroundImage = 'url("file://' + this.settings.getString('dir.music') + this.editedSong.album.coverFilepath + '")';
    this.changedAlbumCover = false;
    this.changedSongFile = false;
    // this does not seem to play well when working straight on the array object
    this.genres = this.editedSong.styles;
    this.forceEdits();
    if (this.settings.getBool('edit.autoAdd')) this.save();
    else if (this.settings.getBool('edit.autoPlay')) setTimeout(() => this.shadowRoot.getElementById('audio').play(), 1);
    this.checkIfSongExists();
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
      if (!this.song.bpm) {
        var songPath = this.song.filepath;
        if (!this.settings.getBool('edit.autoAdd')) ss.AudioAnalyzer.analyzeBpmAsync(this.song)
        .then((candidates) => {
          if (songPath == this.song.filepath) {
            let bpmList = "";
            for (let i = 0; i < 4 && i < candidates.length; ++i) {
              if (i == 0) this.editedSong.bpm = candidates[i].toFixed(2);
              bpmList += candidates[i].toFixed(2) + ", ";
            }
            this.taggedSong.bpm = bpmList;
            this.requestUpdate();
          }
        })
        .catch((err) => this.addAlert("Failed processing BPM: " + err));
      }
      return true;
    }
    // this.sterilizeSong(this.song);  This affects original song, so do not want to change it.
    this.sterilizeSong(this.taggedSong);
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

  prevSong() {
    let event = new CustomEvent('song-list-pos', { detail: { songId: this.song.id, isPrev: true, isEdit: true } });
    this.save();
    window.dispatchEvent(event);
  }

  nextSong() {
    let event = new CustomEvent('song-list-pos', { detail: { songId: this.song.id, isPrev: false, isEdit: true } });
    this.save();
    window.dispatchEvent(event);
  }

  validate() {
    let valid = true;
    let inputs = this.shadowRoot.querySelectorAll('md-filled-text-field');
    for (let i = 0; i < inputs.length; ++i) {
      valid &= inputs[i].reportValidity();
      // if (!inputs[i].checkValidity()) this.addAlert(inputs[i].label + " input is invalid.");
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
      this.albumArtistValChanged(e);
    }
    this.checkIfSongExists();
  }

  titleChanged(e) {
    let title = this.shadowRoot.getElementById('title').value;
    if (!!title &&
        ((!!this.taggedSong.album.name && this.taggedSong.title.toUpperCase() == this.taggedSong.album.name.toUpperCase()) ||
         (!this.taggedSong.album.name && this.taggedSong.title.toUpperCase() == this.song.album.name.toUpperCase()))) {
      this.shadowRoot.getElementById('album_name').value = title;
    }
    this.checkIfSongExists();
  }

  changeCoverFile() {
    let editedSongFilepath = this.settings.getString('dir.music') + this.path.dirname(this.editedSong.filepath);
    ipcRenderer.invoke('opendialog', 'Album Cover', editedSongFilepath, ['openFile'])
    .then((file) => {
      if (!file.canceled) {
        let filepath = file.filePaths[0].toString();
        this.shadowRoot.getElementById('cover').style.backgroundImage = 'url("file://' + filepath + '")';
        this.editedSong.album.coverFilepath = filepath;
        this.changedAlbumCover = true;
      }
    });
  }

  changeSongFile() {
    let editedSongFilepath = this.settings.getString('dir.music') + this.path.dirname(this.editedSong.filepath);
    ipcRenderer.invoke('opendialog', 'Song Path', editedSongFilepath, ['openFile'])
    .then((file) => {
      if (!file.canceled) {
        this.editedSong.filepath = file.filePaths[0].toString();
        this.changedSongFile = true;
      }
    });
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
    if (!!this.song.albumPart) {
      this.shadowRoot.getElementById('albumpart_pos').value = this.song.albumPart.pos;
      this.shadowRoot.getElementById('albumpart_name').value = this.song.albumPart.name.trim();
    } else {
      this.shadowRoot.getElementById('albumpart_pos').value = "";
      this.shadowRoot.getElementById('albumpart_name').value = "";
    }
    this.shadowRoot.getElementById('album_label').value = this.song.album.label.trim();
    this.shadowRoot.getElementById('album_catalogId').value = this.song.album.catalogId.trim();
    this.shadowRoot.getElementById('album_releaseDateYear').value = this.song.album.releaseDateYear;
    this.shadowRoot.getElementById('album_releaseDateMonth').value = this.song.album.releaseDateMonth;
    this.shadowRoot.getElementById('album_releaseDateDay').value = this.song.album.releaseDateDay;
    // my styles component works w/o this crap

    // remove previous validation errors, and early warning for new ones
    let inputs = this.shadowRoot.querySelectorAll('md-filled-text-field');
    for (let i = 0; i < inputs.length; ++i) {
      inputs[i].reportValidity();
    }
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
    song.rating = Number(this.shadowRoot.getElementById('rating').value);
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
    song.album.releaseDateYear = Number(this.shadowRoot.getElementById('album_releaseDateYear').value);
    song.album.releaseDateMonth = Number(this.shadowRoot.getElementById('album_releaseDateMonth').value);
    song.album.releaseDateDay = Number(this.shadowRoot.getElementById('album_releaseDateDay').value);
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
          z-index: 10;
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
          margin: 0 10px;
        }
        .fields > div {
          position: relative;
        }
        .genres {
          height: 372px;
          width: 240px;
          margin: 10px;
        }
        .row {
          display: flex;
          flex-direction: row;
          margin: 10px 0;
        }
        .row > label {
          flex-grow: 1;
          margin: 20px 0;
        }
        md-checkbox {
          margin-right: 10px;
        }
        #bpmBtn {
          position: absolute;
          bottom: -34px;
          right: -8px;
        }

        md-filled-text-field {
          position: relative;
          width: 240px;
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
          top: 14px;
          right: 1px;
          text-align: right;
          color: #666;  /* TODO pull into theme */
          position: absolute;
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
          margin: auto;
          margin-top: 14px;
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
