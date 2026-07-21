import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let keyboardMixin = (superClass) => class extends superClass {

  constructor() {
    super();
    this._keyboardMixinListener = (e) => this.keydownHandler(e);
  }

  connectedCallback() {
    super.connectedCallback();
    window.addEventListener('keydown', this._keyboardMixinListener);
  }

  disconnectedCallback() {
    window.removeEventListener('keydown', this._keyboardMixinListener);
    super.disconnectedCallback();
  }

  validateKeyboardShortcut(e) {
    const target = e.composedPath()[0];
    if (['INPUT', 'TEXTAREA', 'SELECT'].includes(target.tagName) || target.isContentEditable) return false;
    return true;
  }
}

export const KeyboardMixin = dedupingMixin(keyboardMixin);
