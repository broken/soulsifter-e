import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let alertsSubMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      alerts: { type: Array }
    };
  }

  constructor() {
    super();
    this._addAlertListener = e => this._addAlert(e);
    this._rmAlertListener = e => this._rmAlert(e);
    this._updateAlertListener = e => this._updateAlert(e);
    this._unhideAlertsListener = e => this._unhideAlerts(e);

    this.alerts = [];
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('add-alert', this._addAlertListener);
    window.addEventListener('rm-alert', this._rmAlertListener);
    window.addEventListener('update-alert', this._updateAlertListener);
    window.addEventListener('unhide-alerts', this._unhideAlertsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('add-alert', this._addAlertListener);
    window.removeEventListener('rm-alert', this._rmAlertListener);
    window.removeEventListener('update-alert', this._updateAlertListener);
    window.removeEventListener('unhide-alerts', this._unhideAlertsListener);
    super.disconnectedCallback();
  }

  _addAlert(e) {
    this.alerts.push(e.detail);
    this.alertsChanged();
  }

  _rmAlert(e) {
    let idx = this.alerts.findIndex(a => a.id == e.detail);
    this.alerts.splice(idx, 1);
    this.alertsChanged();
  }

  _updateAlert(e) {
    let idx = this.alerts.findIndex(a => a.id == e.detail.id);
    if (!!e.detail.a) this.alerts[idx].msg = e.detail.a;
    if (!!e.detail.progress) this.alerts[idx].progress = e.detail.progress;
    if (!!e.detail.callback) this.alerts[idx].callback = e.detail.callback;
    this.alertsChanged();
  }

  _unhideAlerts() {
    this.alertsChanged();
  }

  alertsChanged() {
  }
}

export const AlertsSubMixin = dedupingMixin(alertsSubMixin);
