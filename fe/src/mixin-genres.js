import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let genresMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      genres: { type: Array }
    };
  }

  constructor() {
    super();
    this._genresListener = (e) => this._genresChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('genres-changed', this._genresListener);
  }

  disconnectedCallback() {
    window.removeEventListener('genres-changed', this._genresListener);
    super.disconnectedCallback();
  }

  changeGenres(x) {
    let event = new CustomEvent('genres-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _genresChanged(e) {
    this.genresChanged(e.detail);
  }

  genresChanged(x) {
    this.genres = x;
  }
}

export const GenresMixin = dedupingMixin(genresMixin);
