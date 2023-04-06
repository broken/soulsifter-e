import { css, html, LitElement } from "lit";

import "./icon-button.js";


class OptionsMenu extends LitElement {
  render() {
    return html`
      <icon-button icon="${this.icon}"></icon-button>
      <div id="menu" elevation="3">
        <slot></slot>
      </div>
    `;
  }

  static get properties() {
    return {
      icon: { type: String },
    }
  }

  constructor() {
    super();
    this.icon = "more_vert";
  }

  static get styles() {
    return [
      css`
        :host {
          position: relative;
        }
        :host([topright]) #menu {
          bottom: 22px;
          top: unset;
          left: 0;
          right: unset;
          display: none;
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
