/**
 * Library for grouping together the functions used for the audio player.
 *
 * Only one of these should exist, and it should be placed on the outermost component,
 * so as to receive audio-player events from all other components.
 *
 * It requires the SettingsMixin to also be used on the component.
 */

import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let audioMixin = (superClass) => class extends superClass {

  constructor() {
    super();
    this.audio = new Audio();
    this._audioPrevSongFilepath = '';
    this._audioPlayer = undefined;
    this.addEventListener('audio-preview-song', this._audioPreviewSongListener);
    this.addEventListener('audio-set-time-pct', this._audioSetTimePctListener);
    this.addEventListener('audio-pause', this._audioPauseListener);
    this.addEventListener('audio-play', this._audioPlayListener);
  }

  connectedCallback() {
    super.connectedCallback();
    this.audio.addEventListener('ended', this._audioEndedListener);
    this.audio.addEventListener('error', this._audioErrorListener);
    this.audio.addEventListener('timeupdate', this._audioTimeUpdateListener);
  }

  disconnectedCallback() {
    this.audio.removeEventListener('ended', this._audioEndedListener);
    this.audio.removeEventListener('error', this._audioErrorListener);
    this.audio.removeEventListener('timeupdate', this._audioTimeUpdateListener);
    super.disconnectedCallback();
  }

  /* listeners for audio object */

  _audioEndedListener = () => {
    if ('audioEnded' in this._audioPlayer) this._audioPlayer.audioEnded();
  };

  _audioErrorListener = mediaError => {
    console.error('Audio error: ' + mediaError.code);
  };

  _audioTimeUpdateListener = () => {
    let currentTime = this.audio.currentTime;
    let progress = currentTime / this.audio.duration;
    if ('progress' in this._audioPlayer) this._audioPlayer.progress = progress;
    if ('currentTime' in this._audioPlayer) this._audioPlayer.currentTime = currentTime;
  };

  /* listeners for audio player */

  _audioPreviewSongListener = async e => {
    let song = e.detail.song;
    let pct = e.detail.pct;
    await this._audioUpdateSrc(song.filepath);
    this._audioPlayer = e.detail.player;
    this._audioChangeCurrentTimePct(pct);
    clearTimeout(this.timeoutId);
    this.audio.play();
    this.timeoutId = setTimeout(() => this.audio.pause(), 1000 * this.settings.getInt('songList.previewTimeInSec'));
  }

  _audioPlayListener = async e => {
    let song = e.detail.song;
    let src = e.detail.src;
    await this._audioUpdateSrc(!!song ? song.filepath : src);
    this._audioPlayer = e.detail.player;
    this.audio.play();
  }

  _audioPauseListener = e => {
    this.audio.pause();
  }

  _audioSetTimePctListener = e => {
    let pct = e.detail.pct;
    this._audioChangeCurrentTimePct(pct);
  }

  /* private member methods */

  async _audioUpdateSrc(filepath) {
    if (filepath == this._audioPrevSongFilepath) return;
    this._audioPrevSongFilepath = filepath;
    if (!!this._audioPlayer && 'audioUnload' in this._audioPlayer) this._audioPlayer.audioUnload();
    // TODO audio source; use local, otherwise contact google api
    filepath = filepath.charAt(0) === '/' ? filepath : this.settings.getString('dir.music') + filepath;
    this.src = 'file://' + filepath;
    this.audio.src = this.src;
    await new Promise((resolve) => {
      const listener = () => {
        this.audio.removeEventListener('durationchange', listener);
        resolve();
      };
      this.audio.addEventListener('durationchange', listener);
    });
  }

  _audioChangeCurrentTimePct(pct) {
    this.audio.currentTime = pct * this.audio.duration;
  }
}

export const AudioMixin = dedupingMixin(audioMixin);
