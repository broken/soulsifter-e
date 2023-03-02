import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-icon";


class StarRating extends LitElement {
  render() {
    let stars = this.stars.map(
      s => !this.isSelected(s) ? html`<mwc-icon @click=${this.select(s)}>star_border</mwc-icon>`
                               : this.isSelected(s) == 1 ? html`<mwc-icon @click=${this.select(s)} selected>star_half</mwc-icon>`
                                                         : html`<mwc-icon @click=${this.select(s)} selected>star</mwc-icon>`);
    return html`
      ${stars}
    `;
  }

  static get properties() {
    return {
      count: { type: Number },
      value: { type: Number },
      max: { type: Number },
      readOnly: { type: Boolean },
      base10: { type: Boolean },
      stars: { type: Array },
    }
  }

  constructor() {
    super();
    this.count = 5;
    this.value = 0;
    this.max = 5;
    this.readOnly = false;
    this.stars = [];
    for (var i = 0; i < this.count; ++i) {
      this.stars.push(i);
    }
  }

  isSelected(star) {
    let ratio = this.max / this.count;
    let weightedStar = star * ratio;
    let full = weightedStar < this.value;
    let half = !full && weightedStar - Math.floor(ratio / 2) < this.value;
    return full ? 2 : half ? 1 : 0;
  }

  select(star) {
    if (!this.readOnly)
      return (e) => this.value = star + 1;
    else
      return () => {};
  }

  static get styles() {
    return [
      css`
        :host {
          white-space: nowrap;
          color: var(--ss-star-rating-color);
        }
        :host(.trashed) {
          color: var(--ss-star-rating-trashed-color);
        }
        mwc-icon {
          --mdc-icon-size: 13px;
          color: inherit;
          vertical-align: middle;
          padding: 0 1px;
        }
        mwc-icon[selected] {
          color: var(--ss-star-rating-selected-color);
        }
      `
    ];
  }
}

window.customElements.define('star-rating', StarRating);
