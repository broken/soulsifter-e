import { css, html, LitElement } from "lit";

import "@polymer/paper-toast";

import { AlertsSubMixin } from "./mixin-alerts-sub.js";
import "./alert-list-item.js";
import "./icon-button.js";


class AlertList extends AlertsSubMixin(LitElement) {
  render() {
    return html`
      <paper-toast id="toast" class="fit-bottom" duration="0">
        ${this.alerts.map(alert => html`
          <alert-list-item .alert="${alert}" .alerts="${this.alerts}"></alert-list-item>
        `)}
      </paper-toast>
    `;
  }

  alertsChanged() {
    if (this.alerts.length > 0) {
      setTimeout(() => { this.shadowRoot.getElementById('toast').open(); }, 10);
    } else {
      this.shadowRoot.getElementById('toast').close();
    }
    this.requestUpdate();
  }
}

window.customElements.define('alert-list', AlertList);
