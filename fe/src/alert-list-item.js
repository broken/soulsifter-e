import { css, html, LitElement } from "lit";

import "@material/mwc-button";
import "@material/mwc-circular-progress";
import "@polymer/paper-toast";

import { AlertsMixin } from "./mixin-alerts.js";
import "./icon-button.js";


class AlertListItem extends AlertsMixin(LitElement) {
  render() {
    return html`
      <div class="msg">
        <mwc-circular-progress progress="${this.alert.progress}" ?indeterminate="${this.alert.progress < 0 || this.alert.progress > 1}" density="-6"></mwc-circular-progress>
        <span>${this.alert.msg}</span>
      </div>
      ${!!this.alert.callback ? html`<mwc-button @click="${this.resolve}" outlined>Resolve</mwc-button>` : ''}
      <icon-button @click=${this.delAlert} icon="clear" id="clear"></icon-button>
    `;
  }

  constructor() {
    super();
  }

  static get properties() {
    return {
      alert: { type: Object },
    }
  }

  delAlert(e) {
    this.rmAlert(this.alert.id);
  }

  alertsChanged(x) {
    this.alerts = x;
    this.requestUpdate();
  }

  resolve(e) {
    this.alert.callback();
  }

  static get styles() {
    return [
      css`
        :host {
          display: flex;
          flex-direction: row;
          justify-content: space-between;
          align-items: center;
        }
        mwc-button {
          --mdc-button-outline-color: white;
          --mdc-theme-on-primary: white;
          --mdc-theme-primary: white;
          margin: -6px 0;
        }
        mwc-circular-progress {
          --mdc-theme-primary: white;
          margin-top: -5px;
          padding-right: 10px;
        }
        .msg {
          display: flex;
          flex-direction: row;
        }
      `
    ];
  }
}

window.customElements.define('alert-list-item', AlertListItem);
