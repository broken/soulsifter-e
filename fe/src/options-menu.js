import { css, html, LitElement } from "@polymer/lit-element";

import "./icon-button.js";


class OptionsMenu extends LitElement {
  render() {
    return html`
      <icon-button icon="more_vert"></icon-button>
      <div id="menu" elevation="3">
        <slot></slot>
      </div>
    `;
  }

  static get styles() {
    return [
      css`
        :host {
          position: relative;
        }
        #menu {
          background-color: var(--ss-options-button-menu-background-color);
          box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
          padding: 15px;
          position: absolute;
          top: 24px;
          right: 4px;
          border-radius: 2px;
          display: none;
          opacity: 0;
          transition: opacity 1s;
          z-index: 1;
        }
        :host(:hover) #menu {
          display: block;
          opacity: 1;
        }
      `
    ];
  }
}

window.customElements.define('options-menu', OptionsMenu);
