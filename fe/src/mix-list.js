import { css, html, LitElement } from "lit";

import { SearchOptionsMixin } from "./mixin-search-options.js";
import { SongMixin } from "./mixin-song.js";
import { } from "./mix-list-item.js";


class MixList extends SearchOptionsMixin(SongMixin(LitElement)) {
  render() {
    let items = this.mixes.map(m => html`<mix-list-item .mix="${m}" ?mvRestrict="${this.searchOptions.mvRestrict}" ?useStems="${this.searchOptions.useStems}"></mix-list-item>`);
    return html`
      ${items}
    `;
  }
  static get properties() {
    return {
      mixes: { type: Array },
      mvRestrict: { type: Boolean },
      useStems: { type: Boolean },
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

  searchOptionsChanged(opts) {
    this.searchOptions = opts;
    if (this.searchOptions.mvRestrict != this.mvRestrict) {
      this.mvRestrict = this.searchOptions.mvRestrict;
      this.shadowRoot.querySelectorAll('mix-list-item').forEach(el => el.mvRestrict = this.mvRestrict);
    }
    if (this.searchOptions.useStems != this.useStems) {
      this.useStems = this.searchOptions.useStems;
      this.shadowRoot.querySelectorAll('mix-list-item').forEach(el => el.useStems = this.useStems);
    }
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
