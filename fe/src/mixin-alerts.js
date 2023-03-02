import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let alertsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      // An alert is : [id, msg, progress]
      alerts: { type: Array }
    };
  }

  constructor() {
    super();
    this._alertsListener = (e) => this._alertsChanged(e);
    this._alertIdPool = 0;
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
    this.alerts.push([++this._alertIdPool, a, progress]);
    this.changeAlerts(this.alerts);
    let id = this._alertIdPool;
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
    return this._alertIdPool;
  }

  rmAlert(id = undefined) {
    if (!!id) {
      let idx = this.alerts.findIndex(a => a[0] == id);
      this.alerts.splice(idx, 1);
    } else {
      this.alerts.shift();
    }
    this.changeAlerts(this.alerts);
  }

  updateAlert(id, progress = undefined, a = undefined, timeoutInSeconds = 0) {
    let idx = this.alerts.findIndex(a => a[0] == id);
    if (!!a) this.alerts[idx][1] = a;
    if (!!progress) this.alerts[idx][2] = progress;
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
    this.changeAlerts(this.alerts);
  }
}

export const AlertsMixin = dedupingMixin(alertsMixin);
