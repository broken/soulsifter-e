import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let queryMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      query: { type: String }
    };
  }

  constructor() {
    super();
    this.query = '';
    this._queryListener = (e) => this._queryChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('query-changed', this._queryListener);
  }

  disconnectedCallback() {
    window.removeEventListener('query-changed', this._queryListener);
    super.disconnectedCallback();
  }

  changeQuery(x) {
    let event = new CustomEvent('query-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _queryChanged(e) {
    this.queryChanged(e.detail);
  }

  queryChanged(x) {
    this.query = x;
  }
}

export const QueryMixin = dedupingMixin(queryMixin);
