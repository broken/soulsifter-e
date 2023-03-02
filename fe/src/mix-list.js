import { css, html, LitElement } from "@polymer/lit-element";

import { SongMixin } from "./mixin-song.js";
import { } from "./mix-list-item.js";


class MixList extends SongMixin(LitElement) {
  render() {
    let items = this.mixes.map(m => html`<mix-list-item .mix="${m}"></mix-list-item>`);
    return html`
      ${items}
    `;
  }
  static get properties() {
    return {
      mixes: { type: Array },
    }
  }

  constructor() {
    super();
    this.mixes = [];
  }

  songChanged(song) {
    this.song = song;
    if (!!song) this.mixes = ss.Mix.findByOutSongId(this.song.id);
    else this.mixes = [];
  }

  static get styles() {
    return [
      css`
        :host {
          overflow-x: hidden;
          overflow-y: scroll;
          max-height: 25vh;
          flex-shrink: 0;
        }
      `
    ];
  }
}

window.customElements.define('mix-list', MixList);
