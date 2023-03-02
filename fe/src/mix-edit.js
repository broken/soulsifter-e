import { css, html, LitElement } from "@polymer/lit-element";

import "@polymer/paper-input/paper-input.js";
import "@polymer/paper-input/paper-textarea.js";

import "./abstract-action-page.js";


class MixEdit extends LitElement {
  render() {
    return html`
      <abstract-action-page @cancel="${this.exit}" @accept="${this.save}">
        <div class="fields">
          <div class="song">&nbsp;<span>Out: ${this.mix.outSong.artist} - ${this.mix.outSong.title}</span></div>
          <div class="song">&nbsp;<span>In: ${this.mix.inSong.artist} - ${this.mix.inSong.title}</span></div>
          <paper-checkbox ?checked="${this.mix.addon}" id="addon">Is Addon</paper-checkbox>
        </div>
        <div class="fields">
          <paper-input floatingLabel label="Rating (0 - 99)" value="${this.mix.rating}" id="rating" auto-validate allowed-pattern="[0-9]"></paper-input>
          <paper-textarea floatingLabel label="Comments" value="${this.mix.comments}" id="comments"></paper-textarea>
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
    this.mix.rating = this.shadowRoot.getElementById('rating').value;
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
      `,
    ];
  }
}

window.customElements.define('mix-edit', MixEdit);
