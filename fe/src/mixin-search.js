import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let searchMixin = (superClass) => class extends superClass {

  constructor() {
    super();
    this._searchListener = () => this.search();
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('search', this._searchListener);
  }

  disconnectedCallback() {
    window.removeEventListener('search', this._searchListener);
    super.disconnectedCallback();
  }

  requestSearch() {
    let event = new CustomEvent('search', { detail: 'request' });
    window.dispatchEvent(event);
  }

  search() {
    /* do nothing */
  }
}

export const SearchMixin = dedupingMixin(searchMixin);
