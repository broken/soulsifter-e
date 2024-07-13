import { css, html, LitElement, unsafeCSS } from 'lit';

import "@material/mwc-tab";
import "@material/mwc-tab-bar";

import { AudioMixin } from "./mixin-audio.js";
import { SettingsMixin } from "./mixin-settings.js";
import "./alert-list.js";
import "./genre-edit.js";
import "./genre-list.js";
import "./playlist-edit.js";
import "./playlist-list.js";
import "./mix-edit.js";
import "./mix-list.js";
import "./search-toolbar.js";
import "./settings-edit.js";
import "./song-edit.js";
import "./song-list.js";
import "./song-section.js";
import "./wave-gen.js"
import "./youtube-client.js"

import 'material-design-icons/iconfont/material-icons.css';
import 'typeface-roboto/index.css';
import theme_default from './css/theme-default.css?inline';
import theme_serato from './css/theme-serato.css?inline';
import theme_traktor from './css/theme-traktor.css?inline';
import greyscalewaveforms from './css/waveforms-greyscale.css?inline';


class SoulSifter extends AudioMixin(SettingsMixin(LitElement)) {
  render() {
    return html`
      <alert-list></alert-list>
      <wave-gen></wave-gen>
      <aside>
        <mwc-tab-bar activeIndex="1" dense="true">
          <mwc-tab @click="${this.songTabClicked}" label="Song" isFadingIndicator></mwc-tab>
          <mwc-tab @click="${this.genresTabClicked}" label="Genres" isFadingIndicator></mwc-tab>
          <mwc-tab @click="${this.playlistsTabClicked}" label="Playlists" isFadingIndicator></mwc-tab>
        </mwc-tab-bar>
        <div id="sections">
          <song-section></song-section>
          <genre-list main class="active"></genre-list>
          <playlist-list></playlist-list>
        </div>
      </aside>
      <main>
        <search-toolbar></search-toolbar>
        <mix-list @select-song="${this.selectSong}"></mix-list>
        <div class="list-seperator"></div>
        <song-list id="sl"></song-list>
      </main>
      <genre-edit></genre-edit>
      <mix-edit></mix-edit>
      <playlist-edit></playlist-edit>
      <settings-edit></settings-edit>
      <song-edit></song-edit>
    `;
  }

  static get properties() {
    return {
      selectedTab: { type: Number },
    };
  }

  constructor() {
    super();
    this.selectedTab = 1;
    this.addEventListener('ss-update-theme', e => {
      if (e.detail.old) {
        e.detail.old.split(',').forEach(theme => {
          this.classList.remove(theme.trim());
        });
      }
      if (e.detail.new) {
        e.detail.new.split(',').forEach(theme => {
          this.classList.add(theme.trim());
        });
      }
    });
  }

  connectedCallback() {
    super.connectedCallback();
    let themes = this.settings.getString('app.theme');
    if (themes) {
      themes.split(',').forEach(theme => {
        this.classList.add(theme.trim());
      });
    }
  }

  tabClicked(tab) {
    if (this.selectedTab != tab) {
      let drawer = this.shadowRoot.getElementById('sections');
      drawer.children.item(tab).classList.toggle('active');
      drawer.children.item(this.selectedTab).classList.toggle('active');
      this.selectedTab = tab;
    }
  }
  songTabClicked() {
    this.tabClicked(0);
  }
  genresTabClicked() {
    this.tabClicked(1);
  }
  playlistsTabClicked() {
    this.tabClicked(2);
  }

  selectSong(e) {
    this.shadowRoot.getElementById('sl').selectSong(e);
  }

  static get styles() {
    return [
      css`${unsafeCSS(theme_default)}`,
      css`${unsafeCSS(theme_serato)}`,
      css`${unsafeCSS(theme_traktor)}`,
      css`${unsafeCSS(greyscalewaveforms)}`,
      css`
        :host {
          display: flex;
          position: absolute;
          top: 0;
          right: 0;
          bottom: 0;
          left: 0;
          color: var(--ss--primary-text-color);
          background-color: var(--ss--primary-background-color);
          font-size: 13px;
          user-select: none;
        }
        main, aside {
          display: flex;
          flex-direction: column;
          height: 100%;
        }
        aside {
          width: var(--sidebar-width);
          flex-grow: 0;
          flex-shrink: 0;
          flex-basis: var(--sidebar-width);
        }
        main {
          flex: 1 1 auto;
        }
        #sections {
          flex: 1 1 auto;
          overflow: hidden;
        }
        mwc-tab-bar {
          background-color: var(--ss-drawer-tab-background-color);
          display: flex;
        }
        mwc-tab {
          --mdc-theme-primary: var(--ss-drawer-tab-active-color);
          --mdc-tab-text-label-color-default: var(--ss-drawer-tab-text-color);
          --mdc-tab-horizontal-padding: 0;
        }
        genre-list {
          transition: transform 1s;
          transform: translate(-100%);
          display: none;
        }
        genre-list.active {
          transition: transform 1s;
          transform: translate(0);
          display: block;
        }
        song-section {
          transition: transform 1s;
          transform: translate(-100%);
          display: none;
        }
        song-section.active {
          transform: translate(0);
          display: block;
        }
        playlist-list {
          transition: transform 1s;
          transform: translate(-100%);
          display: none;
        }
        playlist-list.active {
          transform: translate(0);
          display: block;
        }
        .list-seperator {
          height: 2px;
          background-color: var(--search-toolbar-background);
        }
      `
    ];
  }
}

window.customElements.define('soul-sifter', SoulSifter);
