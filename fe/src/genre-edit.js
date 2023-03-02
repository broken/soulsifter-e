import { css, html, LitElement } from "@polymer/lit-element";

import "@polymer/paper-input/paper-input.js";

import "./abstract-action-page.js";
import "./genre-list.js";


class GenreEdit extends LitElement {
  render() {
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <paper-input floatingLabel label="Name" value="${this.genre.name}" id="name"></paper-input>
        </div>
        <div class="genres">
          <genre-list id="genreList" .genres="${this.genreParents}" singleselect></genre-list>
        </div>
      </abstract-action-page>
    `;
  }

  static get properties() {
    return {
      genre: { type: Object },
      genreParents: { type: Array },
    }
  }

  constructor() {
    super();
    this.genre = new ss.Style();
    this.genreParents = [];
    this.openPageListener = (e) => {
      if (e.detail) {
        this.genre = e.detail;
        this.genreParents = this.genre.parents;
      }
      this.classList.add('show');
    };
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('genre-edit', this.openPageListener);
  }

  disconnectedCallback() {
    window.removeEventListener('genre-edit', this.openPageListener);
    super.disconnectedCallback();
  }

  exit(e) {
    this.classList.remove('show');
  }

  save(e) {
    this.genre.name = this.shadowRoot.getElementById('name').value;
    let genreParentIds = this.genreParents.map(function(g) { return g.id; });
    this.genre.parentIds = genreParentIds;
    if (this.genre.id) {
      this.genre.update();
    } else {
      this.genre.save();
    }
    let event = new CustomEvent('update-genres', { });
    window.dispatchEvent(event);
    this.exit();
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
        }
        :host(.show) {
          display: block;
        }
        .genres {
          padding-left: 30px;
          padding-right: 60px;
          overflow-y: scroll;
          height: 440px;
        }
      `,
    ];
  }
}

window.customElements.define('genre-edit', GenreEdit);
