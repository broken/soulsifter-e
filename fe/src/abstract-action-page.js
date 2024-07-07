import { css, html, LitElement } from "lit";

import "@material/mwc-button";


class AbstractActionPage extends LitElement {
  render() {
    return html`
      <div id="layout">
        <slot></slot>
      </div>
      <div id="abstractActions">
        <mwc-button @click="${this.cancel}" raised>Cancel</mwc-button>
        <mwc-button @click="${this.accept}" raised class="accent">Accept</mwc-button>
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
          background-color: var(--primary-background-color);
          overflow: scroll;
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
        mwc-button {
          padding: 0 30px;
          --mdc-theme-primary: var(--ss-abstract-action-button-bg);
          --mdc-theme-on-primary: var(--ss-abstract-action-button-clr);
        }
        mwc-button.accent {
          --mdc-theme-primary:  var(--ss-abstract-action-button-accent-bg);
          --mdc-theme-on-primary: var(--ss-abstract-action-button-accent-clr);
        }
      `,
    ];
  }
}

window.customElements.define('abstract-action-page', AbstractActionPage);
