import { css, html, LitElement } from "lit";

import "@polymer/paper-toast";

import { AlertsSubMixin } from "./mixin-alerts-sub.js";
import "./alert-list-item.js";


class AlertList extends AlertsSubMixin(LitElement) {
  render() {
    return html`
      <paper-toast id="toast" class="fit-bottom" duration="0">
        ${this.alerts.map(alert => html`
          <alert-list-item .alert="${alert}"></alert-list-item>
        `)}
      </paper-toast>
    `;
  }

  alertsChanged() {
    this.requestUpdate();
  }

  updated(changedProperties) {
    super.updated(changedProperties);
    // imperative dom side effects that depend on reactive data should be inside updated.
    // in other words, the paper-toast must be rendered first, and then we can act on it.
    const toast = this.shadowRoot?.getElementById('toast');
    if (toast) {
      if (this.alerts.length > 0) {
        if (!toast.opened) {
          toast.open();
        }
      } else {
        if (toast.opened) {
          toast.close();
        }
      }
    }
    this.shadowRoot?.querySelectorAll('alert-list-item').forEach(el => el.requestUpdate());
  }
}

window.customElements.define('alert-list', AlertList);
