import { css, html, LitElement } from "lit";

import "@material/web/checkbox/checkbox.js";
import "@material/web/textfield/filled-text-field.js";

import "./abstract-action-page.js";
import { SettingsMixin } from "./mixin-settings.js";


class SettingsEdit extends SettingsMixin(LitElement) {
  render() {
    const rawMidiPattern = "[89A-E,a-e][0-9A-Fa-f] [0-7][0-9A-Fa-f] [x0-7][x0-9A-Fa-f]";
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="outer">
          <mwc-tab-bar activeIndex="0" dense="true">
            <mwc-tab @click="${() => this.tabClicked(0)}" label="Main" isFadingIndicator></mwc-tab>
            <mwc-tab @click="${() => this.tabClicked(1)}" label="Midi" isFadingIndicator></mwc-tab>
            <mwc-tab @click="${() => this.tabClicked(2)}" label="Subscriptions" isFadingIndicator></mwc-tab>
            <mwc-tab @click="${() => this.tabClicked(3)}" label="Auto Drag & Drop" isFadingIndicator></mwc-tab>
          </mwc-tab-bar>
          <main id="main">
            <section active>
              <div class="fields">
                <md-filled-text-field label="Music Directory" .value=${this.musicDir} id="musicDir"></md-filled-text-field>
                <md-filled-text-field label="Music Video Directory" .value=${this.musicVideoDir} id="musicVideoDir"></md-filled-text-field>
                <md-filled-text-field label="Stems Directory" .value=${this.stemsDir} id="stemsDir"></md-filled-text-field>
                <md-filled-text-field label="Waveforms Directory" .value=${this.waveformsDir} id="waveformsDir"></md-filled-text-field>
                <br>
                <label><md-checkbox ?checked="${this.overwriteSongFromTag}" id="overwriteSongFromTag"></md-checkbox>Overwrite values when reading ID3 tags</label>
                <label><md-checkbox ?checked="${this.appDebugMode}" id="appDebugMode"></md-checkbox>Debug mode (requires restart)</label>
              </div>
              <div class="fields">
                <md-filled-text-field label="Default song list limit" .value=${this.songListLimit} allowedPattern="[0-9]*" preventInvalidInput id="songListLimit"></md-filled-text-field></label>
                <md-filled-text-field label="Energy search difference" .value=${this.energyGap} allowedPattern="[0-9]" preventInvalidInput id="energyGap"></md-filled-text-field></label>
                <md-filled-text-field label="Preview Time (in secs)" .value=${this.songListPreviewTimeInSecs} id="songListPreviewTimeInSecs"></md-filled-text-field>
                <label><md-checkbox ?checked="${this.songListSearchOnSelect}" id="songListSearchOnSelect"></md-checkbox>Automatically search after selecting a song</label>
                <label><md-checkbox ?checked="${this.songListShowComments}" id="songListShowComments"></md-checkbox>Show comments in song list</label>
                <label><md-checkbox ?checked="${this.songListShowWaveforms}" id="songListShowWaveforms"></md-checkbox>Show waveforms in song list</label>
                <label><md-checkbox ?checked="${this.includeUnknownKeys}" id="includeUnknownKeys"></md-checkbox>Include unknown keys in key searches</label>
                <label><md-checkbox ?checked="${this.songListColBpm}" id="songListColBpm"></md-checkbox>Include BPM in song list</label>
                <br>
                <label><md-checkbox ?checked="${this.editAutoAdd}" id="editAutoAdd"></md-checkbox>Auto add music (skip edit dialog)</label>
                <label><md-checkbox ?checked="${this.editAutoPlay}" id="editAutoPlay"></md-checkbox>Auto play music when editting song</label>
              </div>
              <div class="fields">
                <md-filled-text-field label="Database Name" .value=${this.dbName} id="dbName"></md-filled-text-field>
                <md-filled-text-field label="Database User" .value=${this.dbUser} id="dbUser"></md-filled-text-field>
                <md-filled-text-field label="Database Password" .value=${this.dbPassword} id="dbPassword"></md-filled-text-field>
                <md-filled-text-field label="Database URL" .value=${this.dbUrl} id="dbUrl"></md-filled-text-field>
                <br>
                <md-filled-text-field label="Theme" .value=${this.appTheme} id="appTheme"></md-filled-text-field>
              </div>
            </section>
            <section>
              <div class="fields">
                <md-filled-text-field label="Midi controller name" .value=${this.midiControllerName} id="midiControllerName"></md-filled-text-field>
                <br>
                <md-filled-text-field label="Browse" .value=${this.midiBrowse} id="midiBrowse" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
                <md-filled-text-field label="Load left deck" .value=${this.midiLoadLeft} id="midiLoadLeft" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
                <md-filled-text-field label="Load right deck" .value=${this.midiLoadRight} id="midiLoadRight" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
                <md-filled-text-field label="Stop audio" .value=${this.midiPauseAudio} id="midiPauseAudio" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
              </div>
              <div class="fields">
                <md-filled-text-field label="Midi volume exp scale" .value=${this.midiVolumeExponentialFactor} id="midiVolumeExponentialFactor"></md-filled-text-field>
                <md-filled-text-field label="Midi volume linear scale" .value=${this.midiVolumeLinearFactor} id="midiVolumeLinearFactor"></md-filled-text-field>
                <md-filled-text-field label="Volume (msb)" .value=${this.midiVolumeMsb} id="midiVolumeMsb" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
                <md-filled-text-field label="Volume (lsb)" .value=${this.midiVolumeLsb} id="midiVolumeLsb" allowedPattern="${rawMidiPattern}"></md-filled-text-field>
              </div>
            </section>
            <section>
              <div class="fields">
                <md-filled-text-field label="Google Music Email" .value=${this.googleEmail} id="googleEmail"></md-filled-text-field>
                <md-filled-text-field label="Google Music App Key" .value=${this.googleAppKey} id="googleAppKey"></md-filled-text-field>
                <md-filled-text-field label="Android ID" .value=${this.googleAndroidId} id="googleAndroidId"></md-filled-text-field>
                <md-filled-text-field label="Google Client ID" .value=${this.googleClientId} id="googleClientId"></md-filled-text-field>
                <md-filled-text-field label="Google Client Secret" .value=${this.googleClientSecret} id="googleClientSecret"></md-filled-text-field>
                <md-filled-text-field label="Google OAuth Refresh Token" .value=${this.googleOauthRefreshToken} id="googleOauthRefreshToken"></md-filled-text-field>
              </div>
              <div class="fields">
                <md-filled-text-field label="Feedly User ID" .value=${this.feedlyUserId} id="feedlyUserId"></md-filled-text-field>
                <md-filled-text-field label="Feedly Developer Token" .value=${this.feedlyDevToken} id="feedlyDevToken"></md-filled-text-field>
              </div>
            </section>
            <section>
              <div class="fields">
                <md-filled-text-field label="Deck A X" .value=${this.dragAndDropDeckLeftX} id="dragAndDropDeckLeftX"></md-filled-text-field>
                <md-filled-text-field label="Deck A Y" .value=${this.dragAndDropDeckLeftY} id="dragAndDropDeckLeftY"></md-filled-text-field>
                <md-filled-text-field label="Deck B X" .value=${this.dragAndDropDeckRightX} id="dragAndDropDeckRightX"></md-filled-text-field>
                <md-filled-text-field label="Deck B Y" .value=${this.dragAndDropDeckRightY} id="dragAndDropDeckRightY"></md-filled-text-field>
                <md-filled-text-field label="D&D Easing" .value=${this.dragAndDropEasing} id="dragAndDropEasing"></md-filled-text-field>
                <md-filled-text-field label="D&D Wait Time (ms)" .value=${this.dragAndDropWaitTimeInMs} id="dragAndDropWaitTimeInMs"></md-filled-text-field>
              </div>
            </section>
          </main>
        </div>
      </abstract-action-page>
    `;
  }

  constructor() {
    super();
    this.selectedTab = 0;
    this.appDebugMode = this.settings.getBool('app.debug');
    this.appTheme = this.settings.getString('app.theme');
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
    this.dragAndDropDeckLeftX = this.settings.getString('dragAndDrop.deckLeftX');
    this.dragAndDropDeckLeftY = this.settings.getString('dragAndDrop.deckLeftY');
    this.dragAndDropDeckRightX = this.settings.getString('dragAndDrop.deckRightX');
    this.dragAndDropDeckRightY = this.settings.getString('dragAndDrop.deckRightY');
    this.dragAndDropEasing = this.settings.getInt('dragAndDrop.easing');
    this.dragAndDropWaitTimeInMs = this.settings.getInt('dragAndDrop.waitTimeInMs');
    this.editAutoAdd = this.settings.getBool('edit.autoAdd');
    this.editAutoPlay = this.settings.getBool('edit.autoPlay');
    this.feedlyDevToken = this.settings.getString('feedly.devToken');
    this.feedlyUserId = this.settings.getString('feedly.userId');
    this.midiBrowse = this.settings.getString('midi.browse');
    this.midiControllerName = this.settings.getString('midi.controllerName');
    this.midiLoadLeft = this.settings.getString('midi.loadLeft');
    this.midiLoadRight = this.settings.getString('midi.loadRight');
    this.midiPauseAudio = this.settings.getString('midi.pauseAudio');
    this.midiVolumeExponentialFactor = this.settings.getString('midi.volume.exponentialFactor');
    this.midiVolumeLinearFactor = this.settings.getString('midi.volume.linearFactor');
    this.midiVolumeLsb = this.settings.getString('midi.volume.lsb');
    this.midiVolumeMsb = this.settings.getString('midi.volume.msb');
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

  tabClicked(tab) {
    if (this.selectedTab != tab) {
      let drawer = this.shadowRoot.getElementById('main');
      drawer.children.item(this.selectedTab).removeAttribute('active');
      drawer.children.item(tab).setAttribute('active', '');
      this.selectedTab = tab;
    }
  }

  exit() {
    this.classList.remove('show');
  }

  save(e, detail, sender) {
    this.validate();
    this.putb('app.debug', this.shadowRoot.getElementById('appDebugMode').checked);
    this.puts('app.theme', this.shadowRoot.getElementById('appTheme').value);
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
    this.puts('dragAndDrop.deckLeftX', this.shadowRoot.getElementById('dragAndDropDeckLeftX').value);
    this.puts('dragAndDrop.deckLeftY', this.shadowRoot.getElementById('dragAndDropDeckLeftY').value);
    this.puts('dragAndDrop.deckRightX', this.shadowRoot.getElementById('dragAndDropDeckRightX').value);
    this.puts('dragAndDrop.deckRightY', this.shadowRoot.getElementById('dragAndDropDeckRightY').value);
    this.puti('dragAndDrop.easing', Number(this.shadowRoot.getElementById('dragAndDropEasing').value));
    this.puti('dragAndDrop.waitTimeInMs', Number(this.shadowRoot.getElementById('dragAndDropWaitTimeInMs').value));
    this.putb('edit.autoAdd', this.shadowRoot.getElementById('editAutoAdd').checked);
    this.putb('edit.autoPlay', this.shadowRoot.getElementById('editAutoPlay').checked);
    this.puts('feedly.devToken', this.shadowRoot.getElementById('feedlyDevToken').value);
    this.puts('feedly.userId', this.shadowRoot.getElementById('feedlyUserId').value);
    this.puts('midi.browse', this.shadowRoot.getElementById('midiBrowse').value);
    this.puts('midi.controllerName', this.shadowRoot.getElementById('midiControllerName').value);
    this.puts('midi.loadLeft', this.shadowRoot.getElementById('midiLoadLeft').value);
    this.puts('midi.loadRight', this.shadowRoot.getElementById('midiLoadRight').value);
    this.puts('midi.pauseAudio', this.shadowRoot.getElementById('midiPauseAudio').value);
    this.puts('midi.volume.exponentialFactor', this.shadowRoot.getElementById('midiVolumeExponentialFactor').value);
    this.puts('midi.volume.linearFactor', this.shadowRoot.getElementById('midiVolumeLinearFactor').value);
    this.puts('midi.volume.lsb', this.shadowRoot.getElementById('midiVolumeLsb').value);
    this.puts('midi.volume.msb', this.shadowRoot.getElementById('midiVolumeMsb').value);
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
          z-index: 10;
          padding-top: 48px;
        }
        :host(.show) {
          display: block;
        }
        abstract-action-page {
          overflow-x: hidden;
          overflow-y: scroll;
        }
        .outer {
          /* margin-top: -22px; */
          width: 100vw;
          display: flex;
          flex-direction: column;
          height: 100%;
          flex-grow: 0;
          flex-shrink: 0;
          min-height: calc(100vh - 102px);
        }
        mwc-tab-bar {
          background-color: var(--ss-drawer-tab-background-color);
          display: flex;
          position: absolute;
          top: 0;
          left: 0;
          right: 0;
        }
        mwc-tab {
          --mdc-theme-primary: var(--ss-drawer-tab-active-color);
          --mdc-tab-text-label-color-default: var(--ss-drawer-tab-text-color);
          --mdc-tab-horizontal-padding: 0;
        }
        main {
          flex-grow: 1;
          flex-shrink: 1;
          flex-basis: auto;
        }
        section {
          transition: transform 1s;
          transform: translate(2000px);
          align-items: flex-start;
          justify-content: center;
          display: none;
        }
        section[active] {
          transition: transform 1s;
          transform: translate(0);
          display: flex;
          flex-direction: row;
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
