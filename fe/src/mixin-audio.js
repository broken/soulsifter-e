/**
 * Library for grouping together the functions used for the audio player.
 *
 * Only one of these should exist, and it should be placed on the core component
 * so as to receive audio-player events from other components.
 *
 * It requires the SettingsMixin also be used on the component.
 */

import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let audioMixin = (superClass) => class extends superClass {

  constructor() {
    super();
    this.audio = new Audio();
    this._audioDuration = 1;
    this._audioPrevSongFilepath = '';
    this.addEventListener('preview-song', (e) => this.previewSong(e));
  }

  connectedCallback() {
    super.connectedCallback();
    this.audio.addEventListener('durationchange', this._audioDurationChangeListener);
    this.audio.addEventListener('ended', this._audioEndedListener);
    this.audio.addEventListener('error', this._audioErrorListener);
    this.audio.addEventListener('timeupdate', this._audioTimeUpdateListener);
  }

  disconnectedCallback() {
    this.audio.removeEventListener('durationchange', this._audioDurationChangeListener);
    this.audio.removeEventListener('ended', this._audioEndedListener);
    this.audio.removeEventListener('error', this._audioErrorListener);
    this.audio.removeEventListener('timeupdate', this._audioTimeUpdateListener);
    super.disconnectedCallback();
  }

  _audioDurationChangeListener = () =>  {
    this._audioDuration = this.audio.duration;
  };

  _audioEndedListener = () => {
    let event = new CustomEvent('song-ended', { bubbles: true, composed: true });
    this.dispatchEvent(event);
  };

  _audioErrorListener = () => {
    console.error('Audio error: ' + mediaError.code);
  };

  _audioTimeUpdateListener = () => {
    this.currentTime = this.audio.currentTime;
  };

  async previewSong(e) {
    let song = e.detail.song;
    let pct = e.detail.pct;
    await this.updateSrc(song.filepath);
    this.changeCurrentTimePct(pct);
    this.preview();
  }

  async updateSrc(filepath) {
    if (filepath == this._audioPrevSongFilepath) return;
    this._audioPrevSongFilepath = filepath;
    // TODO audio source; use local, otherwise contact google api
    filepath = filepath.charAt(0) === '/' ? filepath : this.settings.getString('dir.music') + filepath;
    this.src = 'file://' + filepath;
    this.audio.src = this.src;
  }

  changeCurrentTimePct(pct) {
    this.audio.currentTime = pct * this._audioDuration;
  }

  preview() {
    clearTimeout(this.timeoutId);
    this.audio.play();
    this.timeoutId = setTimeout(() => this.audio.pause(), 10000);
  }
}

export const AudioMixin = dedupingMixin(audioMixin);
