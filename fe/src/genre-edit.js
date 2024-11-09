import { css, html, LitElement } from "lit";

import "@material/web/textfield/filled-text-field.js";

import "./abstract-action-page.js";
import "./genre-list.js";


class GenreEdit extends LitElement {
  render() {
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <md-filled-text-field label="Name" .value="${this.genre.name}" id="name"></md-filled-text-field>
          <md-filled-text-field label="Description" .value="${this.genre.description}" id="description" type="textarea" rows="5"></md-filled-text-field>
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
        this.genre = ss.Style.findById(e.detail.id);
        this.genreParents = this.genre.parents;
        // needed to erase previous descriptions when copying in a genre that doesn't have one
        this.shadowRoot.getElementById('description').value = this.genre.description;
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
    this.genre.description = this.shadowRoot.getElementById('description').value;
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
          z-index: 10;
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
        .fields {
          display: flex;
          flex-direction: column;
          justify-content: center;
          margin: 0 10px;
          width: 360px;
        }
      `,
    ];
  }
}

window.customElements.define('genre-edit', GenreEdit);
