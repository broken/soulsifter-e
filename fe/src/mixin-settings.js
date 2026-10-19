import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

class CachedSoulSifterSettings {
  constructor() {
    this._native = null;
    this._cache = new Map();
  }

  get native() {
    if (!this._native) {
      this._native = new ss.SoulSifterSettings();
    }
    return this._native;
  }

  getString(label) {
    if (!this._cache.has(label)) {
      this._cache.set(label, this.native.getString(label));
    }
    return this._cache.get(label);
  }

  putString(label, value) {
    const val = String(value);
    this._cache.set(label, val);
    this.native.putString(label, val);
  }

  getInt(label) {
    if (!this._cache.has(label)) {
      this._cache.set(label, this.native.getInt(label));
    }
    return this._cache.get(label);
  }

  putInt(label, value) {
    const val = Number(value);
    this._cache.set(label, val);
    this.native.putInt(label, val);
  }

  getBool(label) {
    if (!this._cache.has(label)) {
      this._cache.set(label, this.native.getBool(label));
    }
    return this._cache.get(label);
  }

  putBool(label, value) {
    const val = Boolean(value);
    this._cache.set(label, val);
    this.native.putBool(label, val);
  }

  save() {
    this.native.save();
  }

  clearCache() {
    this._cache.clear();
  }
}

export const settings = new CachedSoulSifterSettings();

let settingsMixin = (superClass) => class extends superClass {
  get settings() {
    return settings;
  }
}

export const SettingsMixin = dedupingMixin(settingsMixin);
