import { css, html, LitElement } from "lit";

import "@material/web/button/filled-button.js";
import "@material/web/button/filled-tonal-button.js";


class AbstractActionPage extends LitElement {
  render() {
    return html`
      <div id="layout">
        <slot></slot>
      </div>
      <div id="abstractActions">
        <md-filled-tonal-button @click="${this.cancel}">Cancel</md-filled-tonal-button>
        <md-filled-button @click="${this.accept}">Accept</md-filled-button>
      </div>
    `;
  }

  accept() {
    let event = new CustomEvent('accept', { });
    this.dispatchEvent(event);
  }

  cancel() {
    let event = new CustomEvent('cancel', { });
    this.dispatchEvent(event);
  }

  static get styles() {
    return [
      css`
        :host {
          height: calc(100% - 2 * 22px);
          padding: 22px;
          display: flex;
          flex-direction: column;
          align-items: center;
          background-color: var(--ss-abstract-action-page-background-color);
          overflow: auto;
        }
        #layout {
          margin-top: auto;
          display: flex;
          flex-direction: row;
          align-items: flex-start;
          justify-content: center;
        }
        #abstractActions {
          margin-bottom: auto;
          padding-top: 30px;
          flex-shrink: 0;
        }
        md-filled-tonal-button {
          margin: 0 20px;
        }
      `,
    ];
  }
}

window.customElements.define('abstract-action-page', AbstractActionPage);
