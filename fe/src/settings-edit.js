import { css, html, LitElement } from "lit";

import "@polymer/paper-checkbox/paper-checkbox.js";
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
          <br>
          <paper-input label="Default song list limit" value="${this.songListLimit}" allowedPattern="[0-9]*" preventInvalidInput id="songListLimit"></paper-input>
          <paper-input label="Energy search difference" value="${this.energyGap}" allowedPattern="[0-9]" preventInvalidInput id="energyGap"></paper-input>
          <paper-checkbox ?checked="${this.songListSearchOnSelect}" id="songListSearchOnSelect">Automatically search after selecting a song</paper-checkbox>
          <paper-checkbox ?checked="${this.songListShowComments}" id="songListShowComments">Show comments in song list</paper-checkbox>
          <paper-checkbox ?checked="${this.includeUnknownKeys}" id="includeUnknownKeys">Include unknown keys in key searches</paper-checkbox>
          <paper-checkbox ?checked="${this.overwriteSongFromTag}" id="overwriteSongFromTag">Overwrite values when reading ID3 tags</paper-checkbox>
          <paper-checkbox ?checked="${this.appDebugMode}" id="appDebugMode">Debug mode (requires restart)</paper-checkbox>
          <paper-checkbox ?checked="${this.editAutoAdd}" id="editAutoAdd">Auto add music (skip edit dialog)</paper-checkbox>
          <paper-checkbox ?checked="${this.editAutoPlay}" id="editAutoPlay">Auto play music when editting song</paper-checkbox>
          <br>
          <paper-checkbox ?checked="${this.songListColBpm}" id="songListColBpm">Include BPM in song list</paper-checkbox>
        </div>
        <div class="fields">
          <paper-input label="Database Name" value="${this.dbName}" id="dbName"></paper-input>
          <paper-input label="Database User" value="${this.dbUser}" id="dbUser"></paper-input>
          <paper-input label="Database Password" value="${this.dbPassword}" id="dbPassword"></paper-input>
          <paper-input label="Database URL" value="${this.dbUrl}" id="dbUrl"></paper-input>
          <br>
          <paper-input label="Feedly User ID" value="${this.feedlyUserId}" id="feedlyUserId"></paper-input>
          <paper-input label="Feedly Developer Token" value="${this.feedlyDevToken}" id="feedlyDevToken"></paper-input>
        </div>
        <div class="fields">
          <paper-input label="Google Music Email" value="${this.googleEmail}" id="googleEmail"></paper-input>
          <paper-input label="Google Music App Key" value="${this.googleAppKey}" id="googleAppKey"></paper-input>
          <paper-input label="Android ID" value="${this.googleAndroidId}" id="googleAndroidId"></paper-input>
          <br>
          <paper-input label="Google Client ID" value="${this.googleClientId}" id="googleClientId"></paper-input>
          <paper-input label="Google Client Secret" value="${this.googleClientSecret}" id="googleClientSecret"></paper-input>
          <paper-input label="Google OAuth Refresh Token" value="${this.googleOauthRefreshToken}" id="googleOauthRefreshToken"></paper-input>
        </div>
      </abstract-action-page>
    `;
  }

  constructor() {
    super();
    this.appDebugMode = this.settings.getBool('app.debug');
    this.musicDir = this.settings.getString('dir.music');
    this.musicVideoDir = this.settings.getString('dir.mv');
    this.stemsDir = this.settings.getString('dir.stems');
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
    this.editAutoAdd = this.settings.getBool('edit.autoAdd');
    this.editAutoPlay = this.settings.getBool('edit.autoPlay');
    this.feedlyDevToken = this.settings.getString('feedly.devToken');
    this.feedlyUserId = this.settings.getString('feedly.userId');
    this.songListColBpm = this.settings.getBool('songList.column.bpm');
    this.songListLimit = this.settings.getInt('songList.limit');
    this.songListSearchOnSelect = this.settings.getBool('songList.searchOnSelect');
    this.songListShowComments = this.settings.getBool('songList.showComments');
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
    this.puts('dir.music', this.shadowRoot.getElementById('musicDir').value);
    this.puts('dir.mv', this.shadowRoot.getElementById('musicVideoDir').value);
    this.puts('dir.stems', this.shadowRoot.getElementById('stemsDir').value);
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
    this.putb('edit.autoAdd', this.shadowRoot.getElementById('editAutoAdd').checked);
    this.putb('edit.autoPlay', this.shadowRoot.getElementById('editAutoPlay').checked);
    this.puts('feedly.devToken', this.shadowRoot.getElementById('feedlyDevToken').value);
    this.puts('feedly.userId', this.shadowRoot.getElementById('feedlyUserId').value);
    this.putb('songList.column.bpm', this.shadowRoot.getElementById('songListColBpm').checked);
    this.puti('songList.limit', Number(this.shadowRoot.getElementById('songListLimit').value));
    this.putb('songList.searchOnSelect', this.shadowRoot.getElementById('songListSearchOnSelect').checked);
    this.putb('songList.showComments', this.shadowRoot.getElementById('songListShowComments').checked);
    this.puti('search.energyGap', Number(this.shadowRoot.getElementById('energyGap').value));
    this.putb('search.includeUnknownKeys', this.shadowRoot.getElementById('includeUnknownKeys').checked);
    this.putb('tag.readOverwrite', this.shadowRoot.getElementById('overwriteSongFromTag').checked);
    this.settings.save();
    this.exit();
  }

  validate() {
    // Ensure that directory paths end in a slash
    ['musicDir', 'musicVideoDir', 'stemsDir'].map(label => {
      let el = this.shadowRoot.getElementById(label);
      el.value = el.value.replace(/(\/)?$/, '/');
    })
  }

  puts(key, value) {
    if (!!key && !!value) {
      this.settings.putString(key, value);
    }
  }

  puti(key, value) {
    if (!!key && (!!value || value === 0)) {
      this.settings.putInt(key, value);
    }
  }

  putb(key, value) {
    if (!!key) {
      this.settings.putBool(key, value);
    }
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
      `,
    ];
  }
}

window.customElements.define('settings-edit', SettingsEdit);
