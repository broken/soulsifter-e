import { css, html, LitElement } from "@polymer/lit-element";

import "@material/mwc-circular-progress";
import "@polymer/paper-toast";

import { AlertsMixin } from "./mixin-alerts.js";
import "./icon-button.js";


class AlertListItem extends AlertsMixin(LitElement) {
  render() {
    return html`
      <div class="msg">
        <mwc-circular-progress progress="${this.alert[2]}" ?indeterminate="${this.alert[2] < 0 || this.alert[2] > 1}" density="-6"></mwc-circular-progress>
        <span>${this.alert[1]}</span>
      </div>
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
    this.rmAlert(this.alert[0]);
  }

  alertsChanged(x) {
    this.alerts = x;
    this.requestUpdate();
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
