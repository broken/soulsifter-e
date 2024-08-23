import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

import { Alert } from "./alert.js";

let alertsMixin = (superClass) => class extends superClass {

  addAlert(a, timeoutInSeconds = 0, progress = 0, callback = undefined) {
    console.log(a);
    let id = Math.random().toString(36).substring(2);
    let alert = new Alert(id, a, progress, callback);
    let event = new CustomEvent('add-alert', { detail: alert });
    window.dispatchEvent(event);
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
    return id;
  }

  rmAlert(id = undefined) {
    let event = new CustomEvent('rm-alert', { detail: id });
    window.dispatchEvent(event);
  }

  updateAlert(id, progress = undefined, a = undefined, timeoutInSeconds = 0, callback = undefined) {
    let event = new CustomEvent('update-alert', { detail: { id: id, progress: progress, a: a, callback: callback } });
    window.dispatchEvent(event);
    if (timeoutInSeconds) setTimeout(() => this.rmAlert(id), timeoutInSeconds * 1000);
  }

  unhideAlerts() {
    let event = new CustomEvent('unhide-alerts');
    window.dispatchEvent(event);
  }
}

export const AlertsMixin = dedupingMixin(alertsMixin);
