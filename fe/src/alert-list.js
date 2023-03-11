import { css, html, LitElement } from "lit";

import "@polymer/paper-toast";

import { AlertsMixin } from "./mixin-alerts.js";
import "./alert-list-item.js";
import "./icon-button.js";


class AlertList extends AlertsMixin(LitElement) {
  render() {
    return html`
      <paper-toast id="toast" class="fit-bottom" duration="0">
        ${this.alerts.map(alert => html`
          <alert-list-item .alert="${alert}"></alert-list-item>
        `)}
      </paper-toast>
    `;
  }

  constructor() {
    super();
    ipcRenderer.on('addalert', (e, data) => {
      this.addAlert(data.a, data.timeoutInSeconds, data.progress);
    });
    ipcRenderer.on('updatealert', (e, data) => {
      this.updateAlert(data.id, data.progress, data.a, data.timeoutInSeconds);
    });
  }

  alertsChanged(x) {
    this.alerts = x;
    if (this.alerts.length > 0) {
      setTimeout(() => { this.shadowRoot.getElementById('toast').open(); }, 10);
    } else {
      this.shadowRoot.getElementById('toast').close();
    }
    this.requestUpdate();
  }

  static get styles() {
    return [
      css`
        #toast {
          --paper-toast-background-color: var(--ss-alert-list-bg-clr);
          --paper-toast-color: white;
        }
      `
    ];
  }
}

window.customElements.define('alert-list', AlertList);
