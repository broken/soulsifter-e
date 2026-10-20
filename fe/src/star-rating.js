import { css, html, LitElement } from "lit";


class StarRating extends LitElement {
  render() {
    let stars = this.stars.map(
      s => !this.isSelected(s) ? html`<span class="material-icons" @click=${this.select(s)}>star_border</span>`
                               : this.isSelected(s) == 1 ? html`<span class="material-icons" @click=${this.select(s)} selected>star_half</span>`
                                                         : html`<span class="material-icons" @click=${this.select(s)} selected>star</span>`);
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
      return (e) => this.value = this.value == star + 1 ? 0 : star + 1;
    else
      return () => {};
  }

  static get styles() {
    return [
      css`
        :host {
          white-space: nowrap;
          color: var(--ss-star-rating-unselected-icon-color);
        }
        :host(.trashed) {
          color: var(--ss-star-rating-trashed-icon-color);
        }
        .material-icons {
          font-family: 'Material Icons';
          font-size: var(--mdc-icon-size, 13px);
          line-height: 1;
          letter-spacing: normal;
          text-transform: none;
          display: inline-block;
          white-space: nowrap;
          word-wrap: normal;
          direction: ltr;
          -webkit-font-smoothing: antialiased;
          text-rendering: optimizeLegibility;
          -moz-osx-font-smoothing: grayscale;
          font-feature-settings: 'liga';
          user-select: none;
          color: inherit;
          vertical-align: middle;
          padding: 0 1px;
        }
        .material-icons[selected] {
          color: var(--ss-star-rating-selected-icon-color);
        }
      `
    ];
  }
}

window.customElements.define('star-rating', StarRating);
