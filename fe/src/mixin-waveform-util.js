import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let waveformUtilMixin = (superClass) => class extends superClass {

  getFullWaveformFilepath(songFilepath) {
    return this.settings.getString('dir.waveforms') + songFilepath.replace(/\.[^.]+$/, '.webp');
  }

  async hasWaveformCachedFile(song) {
    let f = this.getFullWaveformFilepath(song.filepath);
    return ipcRenderer.invoke('existsfilepath', f);
  }
}

export const WaveformUtilMixin = dedupingMixin(waveformUtilMixin);
