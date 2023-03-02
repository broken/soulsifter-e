import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-fab/mwc-fab.js";

import "./genre-list-item.js";
import { GenresMixin } from "./mixin-genres.js";


class GenreList extends GenresMixin(LitElement) {
  render() {
    let genreNames = this.genres.map(g => g.name).join(', ');
    let genreListItems = this.genreParents.map(g => html`<genre-list-item .genre="${g}" @toggle-select="${this.toggleSelection}" ?optionsmenu="${this.main}" ?singleselect="${this.singleSelect}"></genre-list-item>`);
    let fab = this.main ? html`<mwc-fab mini icon="add" @click="${this.createGenre}"></mwc-fab>` : html``;
    return html`
      <div class="gnames">${genreNames}</div>
      <div class="glist">
        ${genreListItems}
      </div>
      ${fab}
    `;
  }

  static get properties() {
    return {
      genreParents: { type: Array },
      singleSelect: { type: Boolean },
      main: { type: Boolean },
    }
  }

  constructor() {
    super();
    this.genreParents = ss.Style.findAllParents().sort((a, b) => a.name.localeCompare(b.name));
    this.singleSelect = false;
    this.main = false;
    if (!this.genres) this.genres = [];
    this.updateGenresListener = (e) => this.genreParents = ss.Style.findAllParents().sort((a, b) => a.name.localeCompare(b.name));
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('update-genres', this.updateGenresListener);
  }

  disconnectedCallback() {
    window.removeEventListener('update-genres', this.updateGenresListener);
    super.disconnectedCallback();
  }

  updated(changedProperties) {
    changedProperties.forEach((oldValue, propName) => {
      if (propName == 'genres') {
        this.genresChanged(this.genres);
      }
    });
  }

  genresChanged(genres) {
    this.genres = genres;
    for (var i = 0, childNodes = this.shadowRoot.querySelectorAll('genre-list-item');
         i < childNodes.length; ++i) {
      childNodes[i].selectIffInArray(this.genres);
    }
  }

  toggleSelection(e) {
    try {
      for (var i = 0; i < this.genres.length; ++i) {
        if (this.genres[i].id == e.detail.genre.id) {
          this.genres.splice(i, 1);
          return;
        }
      }
      // TODO do I really need this?  var g = new ss.Style(detail.genre);
      this.genres.push(e.detail.genre);
    } finally {
      this.requestUpdate();
      if (this.main) this.changeGenres(this.genres);
    }
  }

  createGenre(e, detail, sender) {
    let genre = new ss.Style();
    let event = new CustomEvent('genre-edit', { detail: genre });
    window.dispatchEvent(event);
  }

  static get styles() {
    return [
      css`
        :host {
          padding: 0 0 20px 0;
          height: 100%;
          overflow-x: hidden;
        }
        mwc-fab {
          bottom: 33px;
          position: absolute;
          left: 182px;
        }
        .gnames {
          height: 19px;
          white-space: nowrap;
          overflow: hidden;
          text-overflow: ellipsis;
          border-bottom-style: solid;
          border-bottom-width: thin;
        }
        .glist {
          /* 19px for gnames + 1px for border */
          height: calc(100% - 20px);
          overflow-y: scroll;
        }
      `
    ];
  }
}

window.customElements.define('genre-list', GenreList);
