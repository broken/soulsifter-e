import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-icon";
import "@material/ripple";

class IconButton extends LitElement {
  render() {
    return html`
      <mwc-icon class="mdc-ripple-surface mdc-ripple-surface--primary">${this.icon}</mwc-icon>
    `;
  }

  static get properties() {
    return {
      icon: { type: String },
    }
  }

  static get styles() {
    return [
      css`
        :host {
          width: var(--mdc-icon-size);
          height: var(--mdc-icon-size);
        }
        :host(.active) mwc-icon {
          color: var(--ss-search-info-emphasis-color);
        }
        mwc-icon {
          color: inherit;
          vertical-align: middle;
        }
      `
    ];
  }
}

window.customElements.define('icon-button', IconButton);
