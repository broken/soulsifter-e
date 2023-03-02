import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-tab";
import "@material/mwc-tab-bar";

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
// import "./spotify-client.js";
import "./youtube-client.js";


class SoulSifter extends LitElement {
  render() {
    return html`
      <link id="theme" href="./theme-serato.css" rel="stylesheet">
      <alert-list></alert-list>
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
        <mix-list></mix-list>
        <div class="list-seperator"></div>
        <song-list></song-list>
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

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          position: absolute;
          top: 0;
          right: 0;
          bottom: 0;
          left: 0;
          color: var(--primary-text-color);
          background-color: var(--primary-background-color);
          font-size: 13px;
          user-select: none;
        }
        main, aside {
          display: flex;
          flex-direction: column;
          height: 100%;
        }
        aside {
          width: 256px;
          flex-grow: 0;
          flex-shrink: 0;
          flex-basis: 256px;
        }
        main {
          flex: 1 1 auto;
        }
        #sections {
          flex: 1 1 auto;
          overflow: hidden;
        }
        mwc-tab-bar {
          background-color: var(--ss-drawer-tab-bg);
          display: flex;
        }
        mwc-tab {
          --mdc-theme-on-surface: var(--ss-drawer-tab-clr);
          --mdc-theme-primary: var(--ss-drawer-tab-clr-active);
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
