import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let searchOptionsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      searchOptions: { type: Object }
    };
  }

  constructor() {
    super();
    this.searchOptions = {
      bpmRestrict: false,
      keyRestrict: false,
      trashedRestrict: false,
      repeatRestrict: false,
      mvRestrict: false,
      dynamicGenres: false,
      orderBy: 0,
    };
    this._searchOptionsListener = (e) => this._searchOptionsChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('search-options-changed', this._searchOptionsListener);
  }

  disconnectedCallback() {
    window.removeEventListener('search-options-changed', this._searchOptionsListener);
    super.disconnectedCallback();
  }

  changeSearchOptions(x) {
    let event = new CustomEvent('search-options-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _searchOptionsChanged(e) {
    this.searchOptionsChanged(e.detail);
  }

  searchOptionsChanged(x) {
    this.searchOptions = x;
  }
}

export const SearchOptionsMixin = dedupingMixin(searchOptionsMixin);
