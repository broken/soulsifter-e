import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

import { Alert } from "./alert.js";

let alertsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      alerts: { type: Array }
    };
  }

  constructor() {
    super();
    this._alertsListener = (e) => this._alertsChanged(e);
    this.alerts = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('alerts-changed', this._alertsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('alerts-changed', this._alertsListener);
    super.disconnectedCallback();
  }

  changeAlerts(x) {
    let event = new CustomEvent('alerts-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _alertsChanged(e) {
    this.alertsChanged(e.detail);
  }

  alertsChanged(x) {
    this.alerts = x;
  }

  addAlert(a, timeoutInSeconds = 0, progress = 0) {
    console.log(a);
    let id = Math.random().toString(36).substring(2);
    this.alerts.push(new Alert(id, a, progress));
    this.changeAlerts(this.alerts);
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
    return id;
  }

  rmAlert(id = undefined) {
    if (!!id) {
      let idx = this.alerts.findIndex(a => a.id == id);
      this.alerts.splice(idx, 1);
    } else {
      this.alerts.shift();
    }
    this.changeAlerts(this.alerts);
  }

  updateAlert(id, progress = undefined, a = undefined, timeoutInSeconds = 0, callback = undefined) {
    let idx = this.alerts.findIndex(a => a.id == id);
    if (!!a) this.alerts[idx].msg = a;
    if (!!progress) this.alerts[idx].progress = progress;
    if (!!callback) this.alerts[idx].callback = callback;
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
    this.changeAlerts(this.alerts);
  }

  unhideAlerts() {
    this.changeAlerts(this.alerts);
  }
}

export const AlertsMixin = dedupingMixin(alertsMixin);
