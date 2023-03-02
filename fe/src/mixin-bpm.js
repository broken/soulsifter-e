import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let bpmMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      bpm: { type: String }
    };
  }

  constructor() {
    super();
    this._bpmListener = (e) => this._bpmChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('bpm-changed', this._bpmListener);
  }

  disconnectedCallback() {
    window.removeEventListener('bpm-changed', this._bpmListener);
    super.disconnectedCallback();
  }

  changeBpm(x) {
    let event = new CustomEvent('bpm-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _bpmChanged(e) {
    this.bpmChanged(e.detail);
  }

  bpmChanged(x) {
    this.bpm = x;
  }
}

export const BpmMixin = dedupingMixin(bpmMixin);
