import { css, html, LitElement } from "lit";

import "@material/web/dialog/dialog.js";
import "@polymer/paper-input/paper-input.js";
import "@thomasloven/round-slider";

import { AlertsMixin } from "./mixin-alerts.js";
import { BpmMixin } from "./mixin-bpm.js";
import { QueryMixin } from "./mixin-query.js";
import { SearchMixin } from "./mixin-search.js";
import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SettingsMixin } from "./mixin-settings.js";
import "./about-page.js";
import "./icon-button.js";
import "./options-menu.js";
import "./options-menu-item.js";
import "./search-info.js";
import "./search-options.js";


class SearchToolbar extends AlertsMixin(BpmMixin(QueryMixin(SearchMixin(SearchOptionsMixin(SettingsMixin(LitElement)))))) {
  render() {
    let bpmRestrictBtn = this.searchOptions.bpmRestrict ? html`<icon-button @click=${this.toggleBpmRestrict} icon="music_note" class="active"></icon-button>`
                                                        : html`<icon-button @click=${this.toggleBpmRestrict} icon="music_note"></icon-button>`;
    let keyRestrictBtn = this.searchOptions.keyRestrict ? html`<icon-button @click=${this.toggleKeyRestrict} icon="vpn_key" class="active"></icon-button>`
                                                        : html`<icon-button @click=${this.toggleKeyRestrict} icon="vpn_key"></icon-button>`;
    let stemsBtn = !this.settings.getString('dir.stems') ? html`` :
                   (this.searchOptions.useStems ? html`<icon-button @click=${this.toggleUseStems} icon="dehaze" class="active"></icon-button>`
                                               : html`<icon-button @click=${this.toggleUseStems} icon="dehaze"></icon-button>`);
    let debugMode = this.settings.getBool('app.debug');
    return html`
      <icon-button @click=${this.requestSearch} icon="search"></icon-button>
      <paper-input label="search" value="${this.query}" class="flex" no-label-float @input=${this.queryInputChanged} id="queryInput"></paper-input>
      <icon-button @click=${this.clearSearchAction} icon="clear" id="clear"></icon-button>
      <icon-button @click=${this.openSearchInfoDialog} icon="info_outline"></icon-button>
      ${bpmRestrictBtn}
      ${keyRestrictBtn}
      ${stemsBtn}
      <icon-button @click=${this.openSearchOptionsDialog} icon="build"></icon-button>
      <paper-input label="bpm" value="${this.bpm}" no-label-float @input=${this.bpmInputChanged} id="bpmInput"></paper-input>
      <icon-button @click=${this.tapBpm} icon="hearing"></icon-button>
      <icon-button @click=${this.openCreateSongPage} icon="add_circle" id="createSongButton" @drop="${this.dropCreateSongButton}" @dragover="${this.dragOverCreateSongButton}" @dragleave="${this.dragLeaveCreateSongButton}"></icon-button>
      ${debugMode ? html`<icon-button @click=${this.addSongFromUrl} icon="link"></icon-button>` : ''}
      <icon-button @click=${this.openSettingsPage} icon="settings"></icon-button>
      <round-slider value="${this.volume}" valueLabel="Volume" @value-changing="${this.volumeChanged}" @value-changed="${this.volumeChanged}" min="0" max="100" handleSize="3"></round-slider>
      <options-menu>
        ${debugMode ? html`<options-menu-item @click="${this.addSongFromUrl}">Add Song From URL</options-menu-item>` : ''}
        ${debugMode ? html`<options-menu-item @click="${this.addSongAndVideoFromUrl}">Add Song & Video From URL</options-menu-item>` : ''}
        ${debugMode ? html`<options-menu-item @click="${this.addSongsFromFeedly}">Add Songs From Feedly</options-menu-item>` : ''}
        <options-menu-item @click="${this.updateSongAttributesFromTags}">Update Song Attributes From Tags</options-menu-item>
        <options-menu-item @click="${this.analyzeBpms}">Analyze BPMs</options-menu-item>
        <options-menu-item @click="${this.analyzeDurations}">Analyze Durations</options-menu-item>
        <options-menu-item @click="${this.syncSpotifyPlaylists}"><span style="text-decoration: line-through">Sync Spotify Playlists</span></options-menu-item>
        <options-menu-item @click="${this.syncYouTubePlaylists}">Sync YouTube Playlists</options-menu-item>
        <options-menu-item @click="${this.showHiddenAlerts}">Show hidden alerts</options-menu-item>
        <options-menu-item @click="${this.openAboutPageDialog}">About</options-menu-item>
        ${debugMode ? html`<options-menu-item @click="${this.showDevTools}">View Developer Tools</options-menu-item>` : ''}
      </options-menu>
      <md-dialog id="searchInfoDialog">
        <search-info slot="content"></search-info>
      </md-dialog>
      <md-dialog id="searchOptionsDialog">
        <search-options slot="content"></search-options>
      </md-dialog>
      <md-dialog id="aboutPageDialog">
        <about-page slot="content"></about-page>
      </md-dialog>
    `;
  }

  static get properties() {
    return {
    }
  }

  constructor() {
    super();
    this.bpm = '';
    this.taps = [];
    this.lastTap = Date.now();
    this.volume = 100;
  }

  requestSearch(e) {
    this.changeQuery(this.query);
  }

  queryInputChanged(e) {
    // only update/search if it's not actively being changed
    let query = this.shadowRoot.getElementById('queryInput').value;
    this.query = query;
    setTimeout(() => { if (this.query == query) this.changeQuery(query); }, 750);
  }

  bpmInputChanged(e) {
    // only update if it's not actively being changed
    let bpm = this.shadowRoot.getElementById('bpmInput').value;
    this.bpm = bpm;
    setTimeout(() => { if (this.bpm == bpm) this.changeBpm(bpm); }, 750);
  }

  dragOverCreateSongButton(e) {
    e.preventDefault();
    this.shadowRoot.getElementById('createSongButton').icon = 'add_circle_outline';
    return false;
  }

  dragLeaveCreateSongButton(e) {
    e.preventDefault();
    this.shadowRoot.getElementById('createSongButton').icon = 'add_circle';
    return false;
  }

  dropCreateSongButton(e) {
    e.preventDefault();
    this.shadowRoot.getElementById('createSongButton').icon = 'add_circle';
    var files = e.dataTransfer.files;
    var filepaths = new Array();
    for (var i = 0; i < files.length; ++i) {
      filepaths.push(files[i].path);
    }
    let event = new CustomEvent('song-edit', { detail: { filepaths: filepaths } });
    window.dispatchEvent(event);
    return false;
  }

  analyzeBpms(e) {
    ss.AudioAnalyzer.analyzeBpms();
  }

  analyzeDurations(e) {
    ss.AudioAnalyzer.analyzeDurations();
  }

  updateSongAttributesFromTags(e) {
    let msg = "Updating song attributes from id3 tags.";
    let alertId = this.addAlert(msg, 0, -1);
    ss.TagService.updateSongAttributesFromTags((err, ok, p) => {
      if (!!err) {
        this.updateAlert(alertId, a = '[failed] ' + msg + ' :: ' + err);
      } else if (!!ok) {
        this.updateAlert(alertId, 1, '[done] ' + msg);
      } else {
       this.updateAlert(alertId, p);
      }
    });
  }

  showDevTools(e) {
    ipcRenderer.send('opendevtools');
  }

  syncSpotifyPlaylists(e) {
    window.spotify.syncPlaylists();
  }

  syncYouTubePlaylists(e) {
    window.yt.syncPlaylists();
  }

  showHiddenAlerts(e) {
    this.unhideAlerts();
  }

  addSongFromUrl(e) {
    ipcRenderer.invoke('getclipboard')
    .then((url) => {
      let alertId = this.addAlert('Processing ' + url, undefined, -1);
      let p = ss.MusicVideoService.downloadAudioAsync(url);
      p.then((filepaths) => {
        this.updateAlert(alertId, 1, 'Completed ' + url, undefined, () => {
          this.rmAlert(alertId);
          if (filepaths.length > 0) {
            let event = new CustomEvent('song-edit', { detail: { filepaths: filepaths } });
            window.dispatchEvent(event);
          } else {
            window.console.log('Failed to download audio from url ' + url);
          }
        });
      }).catch((err) => {
        this.updateAlert(alertId, undefined, "Failed processing " + url + "\n" + err);
      });
    });
  }

  addSongAndVideoFromUrl(e) {
    ipcRenderer.invoke('getclipboard')
    .then((url) => {
      window.console.log('Audio url = ' + url);

      var filepaths = ss.MusicVideoService.downloadAudio(url);
      if (filepaths.length > 0) {
        let event = new CustomEvent('song-edit', { detail: { filepaths: filepaths, videoUrl: url } });
        window.dispatchEvent(event);
      } else {
        window.console.log('Failed to download audio from url ' + url);
      }
    });
  }

  addSongsFromFeedly(e) {
    let job = async () => {
      const axios = require("axios");
      let urlBase = 'https://cloud.feedly.com';
      let userId = this.settings.getString('feedly.userId');
      let devToken = this.settings.getString('feedly.devToken');
      axios.defaults.headers.common['Authorization'] = 'Bearer ' + devToken;

      const completed = () => {
        return new Promise((resolve, reject) => {
          window.addEventListener('song-edit-close', resolve, {once: true});
        });
      };

      let response = await axios.get(
          urlBase + '/v3/streams/contents',
          { params: { streamId: 'user/' + userId + '/tag/global.saved', count: 10 } });
      let continuation = response.data.continuation
      for (let entry of response.data.items) {
        let url = entry.alternate[0].href;
        let filepaths = ss.MusicVideoService.downloadAudio(url);
        if (filepaths.length > 0) {
          let event = new CustomEvent('song-edit', { detail: { filepaths: filepaths } });
          window.dispatchEvent(event);
          // wait for completion response event
          let e = await completed();
          if (e.detail) {
            let r = await axios.post(
                urlBase + '/v3/markers',
                { action: 'markAsUnsaved', type: 'entries', entryIds: [ entry.id ] });
            console.log('Unsaved song.');
            console.log(entry);
          }
        } else {
          throw new Error('Failed to download audio from url ' + url);
        }
      }
    };
    job().then(() => console.log('success')).catch(err => console.warn(err));
  }

  openSearchInfoDialog(e) {
    this.shadowRoot.getElementById('searchInfoDialog').show();
  }

  openSearchOptionsDialog(e) {
    this.shadowRoot.getElementById('searchOptionsDialog').show();
  }

  openAboutPageDialog(e) {
    this.shadowRoot.getElementById('aboutPageDialog').show();
  }

  toggleBpmRestrict(e) {
    this.searchOptions.bpmRestrict = !this.searchOptions.bpmRestrict;
    this.changeSearchOptions(this.searchOptions);
  }

  toggleKeyRestrict(e) {
    this.searchOptions.keyRestrict = !this.searchOptions.keyRestrict;
    this.changeSearchOptions(this.searchOptions);
  }

  toggleUseStems(e) {
    this.searchOptions.useStems = !this.searchOptions.useStems;
    this.changeSearchOptions(this.searchOptions);
  }

  openSettingsPage(e) {
    let event = new CustomEvent('settings-edit', { });
    window.dispatchEvent(event);
  }

  openCreateSongPage(e) {
    let song = new ss.Song();
    let event = new CustomEvent('song-edit', { detail: { song: song } });
    window.dispatchEvent(event);
  }

  clearSearchAction(e) {
    this.query = '';
    this.shadowRoot.getElementById('queryInput').value = this.query;
    this.changeQuery(this.query);
  }

  tapBpm(e) {
    const now = Date.now();
    const diff = now - this.lastTap;
    this.lastTap = now;
    console.log(diff);

    if (diff > 1300) {  // 1300 would be 46 bpm
      this.taps = [];
      return;
    }

    this.taps.push(diff);
    if (this.taps.length > 32) {
      this.taps.shift();
    }

    const avg = this.taps.reduce((a, b) => a + b) / this.taps.length;
    this.changeBpm((60000 / avg).toFixed(1));
  }

  volumeChanged(e) {
    let vol = e.detail.value / 100;
    let event = new CustomEvent(
      'audio-set-volume',
      { bubbles: true, composed: true, detail: { volume: vol }}
    );
    this.dispatchEvent(event);
  }

  static get styles() {
    return [
      css`
        :host {
          background-color: var(--ss-search-toolbar-bg);
          display: flex;
          flex-direction: row;
          height: 42px;
          padding-top: 5px;
          padding-left: 17px;
          box-shadow: 0 2px 4px -1px rgba(0,0,0,.2), 0 4px 5px 0 rgba(0,0,0,.14), 0 1px 10px 0 rgba(0,0,0,.12);
        }
        icon-button {
          --mdc-icon-size: 24px;
        }
        icon-button, options-menu {
          padding: 8px;
        }
        #searchInfoDialog {
          max-width: none;
        }
        #searchOptionsDialog {
          max-width: none;
        }
        #bpmInput {
          width: 64px;
        }
        .flex {
          flex: 1;
          flex-basis: auto;
        }
        .menuItem {
          white-space: nowrap;
        }
        #clear {
          padding: 8px 0;
          margin-left: -24px;
          z-index: 0;
        }
        round-slider {
          padding: 7px;
          max-width: 29px;
        }
      `
    ];
  }
}

window.customElements.define('search-toolbar', SearchToolbar);
