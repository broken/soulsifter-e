import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let settingsMixin = (superClass) => class extends superClass {

  static get properties() {
    return {
      settings: { type: Object }
    };
  }

  constructor() {
    super();
    this.settings = new ss.SoulSifterSettings();
  }
}

export const SettingsMixin = dedupingMixin(settingsMixin);
