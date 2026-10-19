import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';
import { SettingsMixin } from './mixin-settings.js';

const os = window.require('os');
const path = window.require('path');

let waveformUtilMixin = (superClass) => class extends SettingsMixin(superClass) {

  // should match css values in song-list-item.js
  waveformHeight = 24;
  waveformWidth = 200;

  getWaveformsBaseDir(songFilepath) {
    if (ss.Song.isTemporaryFilepath(songFilepath)) {
      const tmpBase = this.settings.getString('dir.tmp') || os.tmpdir() || '/tmp';
      return path.join(tmpBase, 'soulsifter-waveforms');
    }
    return this.settings.getString('dir.waveforms');
  }

  getFullWaveformFilepath(songFilepath) {
    const baseDir = this.getWaveformsBaseDir(songFilepath);
    const relSongPath = songFilepath.replace(/^([a-zA-Z]:[\\/]|[/])/, '');
    const wfRelPath = relSongPath.replace(/\.[^.]+$/, `_${this.waveformHeight}x${this.waveformWidth}.webp`);
    return path.join(baseDir, wfRelPath);
  }

  async hasWaveformCachedFile(song) {
    let f = this.getFullWaveformFilepath(song.filepath);
    return ipcRenderer.invoke('existsfilepath', f);
  }
}

export const WaveformUtilMixin = dedupingMixin(waveformUtilMixin);
