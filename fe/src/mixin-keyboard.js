import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let keyboardMixin = (superClass) => class extends superClass {

  validateKeyboardShortcut(e) {
    const target = e.composedPath()[0];
    if (['INPUT', 'TEXTAREA', 'SELECT'].includes(target.tagName) || target.isContentEditable) return false;
    return true;
  }
}

export const KeyboardMixin = dedupingMixin(keyboardMixin);
