import { css, html, LitElement } from "lit";

import "@material/web/checkbox/checkbox.js";
import "@material/web/textfield/filled-text-field.js";

import "./abstract-action-page.js";


class MixEdit extends LitElement {
  render() {
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <div class="song">&nbsp;<span>Out: ${this.mix.outSong.artist} - ${this.mix.outSong.title}</span></div>
          <div class="song">&nbsp;<span>In: ${this.mix.inSong.artist} - ${this.mix.inSong.title}</span></div>
          <div class="row">
            <md-filled-text-field label="Rating (0 - 99)" .value="${this.mix.rating}" id="rating" pattern="[0-9]"></md-filled-text-field>
            <label><md-checkbox ?checked="${this.mix.addon}" id="addon"></md-checkbox>Is Addon</label>
          </div>
          <md-filled-text-field type="textarea" label="Comments" .value="${this.mix.comments}" id="comments" rows="10"></md-filled-text-field>
        </div>
      </abstract-action-page>
    `;
  }

  static get properties() {
    return {
      mix: { type: Object },
    }
  }

  constructor() {
    super();
    this.mix = new ss.Mix();
    this.mix.inSong = new ss.Song();
    this.mix.outSong = new ss.Song();
    this.openPageListener = (e) => {
      this.mix = e.detail;
      this.classList.add('show');
      // forceEdits
      this.shadowRoot.getElementById('comments').value = '';
      this.shadowRoot.getElementById('rating').value = '';
    };
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('mix-edit', this.openPageListener);
  }

  disconnectedCallback() {
    window.removeEventListener('mix-edit', this.openPageListener);
    super.disconnectedCallback();
  }

  exit(e) {
    this.classList.remove('show');
  }

  save(e) {
    this.mix.comments = this.shadowRoot.getElementById('comments').value;
    this.mix.rating = Number(this.shadowRoot.getElementById('rating').value);
    this.mix.addon = this.shadowRoot.getElementById('addon').checked;
    if (this.mix.id) {
      this.mix.update();
    } else {
      this.mix.save();
    }
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
        .fields {
          display: flex;
          flex-direction: column;
          justify-content: center;
          margin: 0 100px;
        }
        .song {
          margin: 15px;
        }
        md-checkbox {
          margin-right: 10px;
        }
        .row {
          display: flex;
          flex-direction: row;
          margin: 10px 0;
          justify-content: space-between;
        }
        .row > label {
          flex-grow: 1;
          margin: 20px 0;
        }
      `,
    ];
  }
}

window.customElements.define('mix-edit', MixEdit);
