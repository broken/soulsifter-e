import { css, html, LitElement } from "lit";

import "@material/mwc-ripple";


class OptionsMenuItem extends LitElement {
  render() {
    return html`
      <slot></slot>
      <!-- mwc-ripple primary></mwc-ripple -->
      <!-- Leaving ripple here even though it doesn't work when its state is initially hidden. -->
      <!-- Ripple bug: https://github.com/material-components/material-components-web-components/issues/200 -->
    `;
  }

  static get styles() {
    return [
      css`
        :host {
          line-height: 22px;
          margin: 2px;
          position: relative;
          display: block;
          white-space: nowrap;
        }
        :host(:hover) {
          color: var(--ss-options-menu-text-color-hover);
          background-color: var(--ss-options-menu-background-color-hover);
          cursor: pointer;
        }
        :host([hidden]) {
          display: none;
        }
      `
    ];
  }
}

window.customElements.define('options-menu-item', OptionsMenuItem);
