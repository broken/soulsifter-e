import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let songEditMixin = (superClass) => class extends superClass {

  constructor() {
    super();
    this._songEditListener = (e) => this._songEditChanged(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('song-edit-changed', this._songEditListener);
  }

  disconnectedCallback() {
    window.removeEventListener('song-edit-changed', this._songEditListener);
    super.disconnectedCallback();
  }

  notifySongEdited(x) {
    let event = new CustomEvent('song-edit-changed', { detail: x });
    window.dispatchEvent(event);
  }

  _songEditChanged(e) {
    this.updateEditedSong(e.detail);
  }

  updateEditedSong(x) {
  }
}

export const SongEditMixin = dedupingMixin(songEditMixin);
