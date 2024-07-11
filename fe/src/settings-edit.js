import { css, html, LitElement } from "lit";

import "@material/web/checkbox/checkbox.js";
import "@polymer/paper-input/paper-input.js";

import "./abstract-action-page.js";
import { SettingsMixin } from "./mixin-settings.js";


class SettingsEdit extends SettingsMixin(LitElement) {
  render() {
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <paper-input label="Music Directory" value="${this.musicDir}" id="musicDir"></paper-input>
          <paper-input label="Music Video Directory" value="${this.musicVideoDir}" id="musicVideoDir"></paper-input>
          <paper-input label="Stems Directory" value="${this.stemsDir}" id="stemsDir"></paper-input>
          <paper-input label="Waveforms Directory" value="${this.waveformsDir}" id="waveformsDir"></paper-input>
          <br>
          <paper-input label="Default song list limit" value="${this.songListLimit}" allowedPattern="[0-9]*" preventInvalidInput id="songListLimit"></paper-input></label>
          <paper-input label="Energy search difference" value="${this.energyGap}" allowedPattern="[0-9]" preventInvalidInput id="energyGap"></paper-input></label>
          <label><md-checkbox ?checked="${this.songListSearchOnSelect}" id="songListSearchOnSelect"></md-checkbox>Automatically search after selecting a song</label>
          <label><md-checkbox ?checked="${this.songListShowComments}" id="songListShowComments"></md-checkbox>Show comments in song list</label>
          <label><md-checkbox ?checked="${this.songListShowWaveforms}" id="songListShowWaveforms"></md-checkbox>Show waveforms in song list</label>
          <label><md-checkbox ?checked="${this.includeUnknownKeys}" id="includeUnknownKeys"></md-checkbox>Include unknown keys in key searches</label>
          <label><md-checkbox ?checked="${this.overwriteSongFromTag}" id="overwriteSongFromTag"></md-checkbox>Overwrite values when reading ID3 tags</label>
          <label><md-checkbox ?checked="${this.appDebugMode}" id="appDebugMode"></md-checkbox>Debug mode (requires restart)</label>
          <label><md-checkbox ?checked="${this.editAutoAdd}" id="editAutoAdd"></md-checkbox>Auto add music (skip edit dialog)</label>
          <label><md-checkbox ?checked="${this.editAutoPlay}" id="editAutoPlay"></md-checkbox>Auto play music when editting song</label>
          <br>
          <label><md-checkbox ?checked="${this.songListColBpm}" id="songListColBpm"></md-checkbox>Include BPM in song list</label>
        </div>
        <div class="fields">
          <paper-input label="Database Name" value="${this.dbName}" id="dbName"></paper-input>
          <paper-input label="Database User" value="${this.dbUser}" id="dbUser"></paper-input>
          <paper-input label="Database Password" value="${this.dbPassword}" id="dbPassword"></paper-input>
          <paper-input label="Database URL" value="${this.dbUrl}" id="dbUrl"></paper-input>
          <br>
          <paper-input label="Theme" value="${this.appTheme}" id="appTheme"></paper-input>
          <br>
          <paper-input label="Preview Time (in secs)" value="${this.songListPreviewTimeInSecs}" id="songListPreviewTimeInSecs"></paper-input>
          <br>
          <paper-input label="Deck A X" value="${this.dragAndDropDeckLeftX}" id="dragAndDropDeckLeftX"></paper-input>
          <paper-input label="Deck A Y" value="${this.dragAndDropDeckLeftY}" id="dragAndDropDeckLeftY"></paper-input>
          <paper-input label="Deck B X" value="${this.dragAndDropDeckRightX}" id="dragAndDropDeckRightX"></paper-input>
          <paper-input label="Deck B Y" value="${this.dragAndDropDeckRightY}" id="dragAndDropDeckRightY"></paper-input>
          <paper-input label="D&D Easing" value="${this.dragAndDropEasing}" id="dragAndDropEasing"></paper-input>
          <paper-input label="D&D Wait Time (ms)" value="${this.dragAndDropWaitTimeInMs}" id="dragAndDropWaitTimeInMs"></paper-input>
          <paper-input label="Midi volume channel" value="${this.audioVolumeMidiChannel}" id="audioVolumeMidiChannel"></paper-input>
          <paper-input label="Midi volume cc (msb)" value="${this.audioVolumeMidiCC}" id="audioVolumeMidiCC"></paper-input>
          <paper-input label="Midi volume exp scale" value="${this.audioExponentialFactor}" id="audioExponentialFactor"></paper-input>
          <paper-input label="Midi volume linear scale" value="${this.audioLinearFactor}" id="audioLinearFactor"></paper-input>
        </div>
        <div class="fields">
          <paper-input label="Google Music Email" value="${this.googleEmail}" id="googleEmail"></paper-input>
          <paper-input label="Google Music App Key" value="${this.googleAppKey}" id="googleAppKey"></paper-input>
          <paper-input label="Android ID" value="${this.googleAndroidId}" id="googleAndroidId"></paper-input>
          <br>
          <paper-input label="Google Client ID" value="${this.googleClientId}" id="googleClientId"></paper-input>
          <paper-input label="Google Client Secret" value="${this.googleClientSecret}" id="googleClientSecret"></paper-input>
          <paper-input label="Google OAuth Refresh Token" value="${this.googleOauthRefreshToken}" id="googleOauthRefreshToken"></paper-input>
          <br>
          <paper-input label="Feedly User ID" value="${this.feedlyUserId}" id="feedlyUserId"></paper-input>
          <paper-input label="Feedly Developer Token" value="${this.feedlyDevToken}" id="feedlyDevToken"></paper-input>
        </div>
      </abstract-action-page>
    `;
  }

  constructor() {
    super();
    this.appDebugMode = this.settings.getBool('app.debug');
    this.appTheme = this.settings.getString('app.theme');
    this.audioExponentialFactor = this.settings.getString('audio.exponentialFactor');
    this.audioLinearFactor = this.settings.getString('audio.linearFactor');
    this.audioVolumeMidiCC = this.settings.getInt('audio.volumeMidiCC');
    this.audioVolumeMidiChannel = this.settings.getInt('audio.volumeMidiChannel');
    this.musicDir = this.settings.getString('dir.music');
    this.musicVideoDir = this.settings.getString('dir.mv');
    this.stemsDir = this.settings.getString('dir.stems');
    this.waveformsDir = this.settings.getString('dir.waveforms');
    this.googleAndroidId = this.settings.getString('google.androidId');
    this.googleAppKey = this.settings.getString('google.appKey');
    this.googleEmail = this.settings.getString('google.email');
    this.googleClientId = this.settings.getString('google.clientId');
    this.googleClientSecret = this.settings.getString('google.clientSecret');
    this.googleOauthRefreshToken = this.settings.getString('google.oauthRefreshToken');
    this.dbName = this.settings.getString('db.name');
    this.dbUser = this.settings.getString('db.user');
    this.dbPassword = this.settings.getString('db.password');
    this.dbUrl = this.settings.getString('db.url');
    this.dragAndDropDeckLeftX = this.settings.getInt('dragAndDrop.deckLeftX');
    this.dragAndDropDeckLeftY = this.settings.getInt('dragAndDrop.deckLeftY');
    this.dragAndDropDeckRightX = this.settings.getInt('dragAndDrop.deckRightX');
    this.dragAndDropDeckRightY = this.settings.getInt('dragAndDrop.deckRightY');
    this.dragAndDropEasing = this.settings.getInt('dragAndDrop.easing');
    this.dragAndDropWaitTimeInMs = this.settings.getInt('dragAndDrop.waitTimeInMs');
    this.editAutoAdd = this.settings.getBool('edit.autoAdd');
    this.editAutoPlay = this.settings.getBool('edit.autoPlay');
    this.feedlyDevToken = this.settings.getString('feedly.devToken');
    this.feedlyUserId = this.settings.getString('feedly.userId');
    this.songListColBpm = this.settings.getBool('songList.column.bpm');
    this.songListLimit = this.settings.getInt('songList.limit');
    this.songListPreviewTimeInSecs = this.settings.getInt('songList.previewTimeInSec');
    this.songListSearchOnSelect = this.settings.getBool('songList.searchOnSelect');
    this.songListShowComments = this.settings.getBool('songList.showComments');
    this.songListShowWaveforms = this.settings.getBool('songList.showWaveforms');
    this.energyGap = this.settings.getInt('search.energyGap');
    this.includeUnknownKeys = this.settings.getBool('search.includeUnknownKeys');
    this.overwriteSongFromTag = this.settings.getBool('tag.readOverwrite');
    this.settingsEditListener = (e) => this.classList.add('show');
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('settings-edit', this.settingsEditListener);
  }

  disconnectedCallback() {
    window.removeEventListener('settings-edit', this.settingsEditListener);
    super.disconnectedCallback();
  }

  exit() {
    this.classList.remove('show');
  }

  save(e, detail, sender) {
    this.validate();
    this.putb('app.debug', this.shadowRoot.getElementById('appDebugMode').checked);
    this.puts('app.theme', this.shadowRoot.getElementById('appTheme').value);
    this.puts('audio.exponentialFactor', this.shadowRoot.getElementById('audioExponentialFactor').value);
    this.puts('audio.linearFactor', this.shadowRoot.getElementById('audioLinearFactor').value);
    this.puti('audio.volumeMidiCC', Number(this.shadowRoot.getElementById('audioVolumeMidiCC').value));
    this.puti('audio.volumeMidiChannel', Number(this.shadowRoot.getElementById('audioVolumeMidiChannel').value));
    this.puts('dir.music', this.shadowRoot.getElementById('musicDir').value);
    this.puts('dir.mv', this.shadowRoot.getElementById('musicVideoDir').value);
    this.puts('dir.stems', this.shadowRoot.getElementById('stemsDir').value);
    this.puts('dir.waveforms', this.shadowRoot.getElementById('waveformsDir').value);
    this.puts('google.androidId', this.shadowRoot.getElementById('googleAndroidId').value);
    this.puts('google.appKey', this.shadowRoot.getElementById('googleAppKey').value);
    this.puts('google.email', this.shadowRoot.getElementById('googleEmail').value);
    this.puts('google.clientId', this.shadowRoot.getElementById('googleClientId').value);
    this.puts('google.clientSecret', this.shadowRoot.getElementById('googleClientSecret').value);
    this.puts('google.oauthRefreshToken', this.shadowRoot.getElementById('googleOauthRefreshToken').value);
    this.puts('db.name', this.shadowRoot.getElementById('dbName').value);
    this.puts('db.user', this.shadowRoot.getElementById('dbUser').value);
    this.puts('db.password', this.shadowRoot.getElementById('dbPassword').value);
    this.puts('db.url', this.shadowRoot.getElementById('dbUrl').value);
    this.puti('dragAndDrop.deckLeftX', Number(this.shadowRoot.getElementById('dragAndDropDeckLeftX').value));
    this.puti('dragAndDrop.deckLeftY', Number(this.shadowRoot.getElementById('dragAndDropDeckLeftY').value));
    this.puti('dragAndDrop.deckRightX', Number(this.shadowRoot.getElementById('dragAndDropDeckRightX').value));
    this.puti('dragAndDrop.deckRightY', Number(this.shadowRoot.getElementById('dragAndDropDeckRightY').value));
    this.puti('dragAndDrop.easing', Number(this.shadowRoot.getElementById('dragAndDropEasing').value));
    this.puti('dragAndDrop.waitTimeInMs', Number(this.shadowRoot.getElementById('dragAndDropWaitTimeInMs').value));
    this.putb('edit.autoAdd', this.shadowRoot.getElementById('editAutoAdd').checked);
    this.putb('edit.autoPlay', this.shadowRoot.getElementById('editAutoPlay').checked);
    this.puts('feedly.devToken', this.shadowRoot.getElementById('feedlyDevToken').value);
    this.puts('feedly.userId', this.shadowRoot.getElementById('feedlyUserId').value);
    this.putb('songList.column.bpm', this.shadowRoot.getElementById('songListColBpm').checked);
    this.puti('songList.limit', Number(this.shadowRoot.getElementById('songListLimit').value));
    this.puti('songList.previewTimeInSec', Number(this.shadowRoot.getElementById('songListPreviewTimeInSecs').value));
    this.putb('songList.searchOnSelect', this.shadowRoot.getElementById('songListSearchOnSelect').checked);
    this.putb('songList.showComments', this.shadowRoot.getElementById('songListShowComments').checked);
    this.putb('songList.showWaveforms', this.shadowRoot.getElementById('songListShowWaveforms').checked);
    this.puti('search.energyGap', Number(this.shadowRoot.getElementById('energyGap').value));
    this.putb('search.includeUnknownKeys', this.shadowRoot.getElementById('includeUnknownKeys').checked);
    this.putb('tag.readOverwrite', this.shadowRoot.getElementById('overwriteSongFromTag').checked);
    this.settings.save();
    this.exit();
  }

  validate() {
    // Ensure that directory paths end in a slash
    ['musicDir', 'musicVideoDir', 'stemsDir', 'waveformsDir'].map(label => {
      let el = this.shadowRoot.getElementById(label);
      el.value = el.value.replace(/(\/)?$/, '/');
    })
    // Replace theme
    let event = new CustomEvent(
      'ss-update-theme',
      {
        bubbles: true,
        composed: true,
        detail: {
          'old': this.settings.getString('app.theme'),
          'new': this.shadowRoot.getElementById('appTheme').value
        }
      }
    );
    this.dispatchEvent(event);
  }

  puts(key, value) {
    this.settings.putString(key, value);
  }

  puti(key, value) {
    this.settings.putInt(key, value);
  }

  putb(key, value) {
    this.settings.putBool(key, value);
  }

  static get styles() {
    return [
      css`
        :host {
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
        .fields {
          display: flex;
          flex-direction: column;
          justify-content: center;
          margin: 0 100px;
        }
        md-checkbox {
          margin-right: 10px;
        }
      `,
    ];
  }
}

window.customElements.define('settings-edit', SettingsEdit);
