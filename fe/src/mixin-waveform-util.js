import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let waveformUtilMixin = (superClass) => class extends superClass {

  // should match css values in song-list-item.js
  waveformHeight = 24;
  waveformWidth = 200;

  getFullWaveformFilepath(songFilepath) {
    return this.settings.getString('dir.waveforms') + songFilepath.replace(/\.[^.]+$/, '_' + this.waveformHeight + 'x' + this.waveformWidth + '.webp');
  }

  async hasWaveformCachedFile(song) {
    let f = this.getFullWaveformFilepath(song.filepath);
    return ipcRenderer.invoke('existsfilepath', f);
  }
}

export const WaveformUtilMixin = dedupingMixin(waveformUtilMixin);
