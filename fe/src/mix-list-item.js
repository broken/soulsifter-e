import { css, html, LitElement } from "@polymer/lit-element";

import "./icon-button.js";
import "./song-list-item.js";
import "./star-rating.js";


class MixListItem extends LitElement {
  render() {
    let once = this.mix.rating < 50;
    let normalizedValue = this.mix.rating - (once ? 0 : 50);
    let rating = Math.round(normalizedValue / 5);
    return html`
      <song-list-item .song="${this.mix.inSong}"></song-list-item>
      <div class="info">
        <star-rating value="${rating}" readonly max="10" ?once="${once}"></star-rating>
        <div class="comments">${this.mix.comments}</div>
        <icon-button icon="edit" class="edit" @click="${this.openEditMixPage}"></icon-button>
      </div>
    `;
  }

  static get properties() {
    return {
      mix: { type: Object },
    };
  }

  constructor() {
    super();
  }

  openEditMixPage(e) {
    let event = new CustomEvent('mix-edit', { detail: this.mix });
    window.dispatchEvent(event);
    e.stopPropagation();
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: column;
        }
        star-rating {
          margin: 0 6px;
        }
        star-rating[once] {
          filter: brightness(0.6) sepia(1) brightness(1.1) hue-rotate(-25deg);
        }
        .info {
          display: flex;
          flex-direction: row;
          align-items: center;
          height: 30px;
          padding: 0 8px;
          margin: 0 10px 0 60px;
          border-bottom: var(--ss-song-list-item-border-bottom);
        }
        .comments {
          max-width: calc(100% - 115px);
        }
        .edit {
          padding-left: 14px;
          display: none;
          --mdc-icon-size: 16px;
          color: var(--primary-text-color);
        }
        .info:hover > .edit {
          display: block;
        }
      `,
    ];
  }
}

window.customElements.define('mix-list-item', MixListItem);
