import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

import { SettingsMixin } from './mixin-settings.js';

const { ipcRenderer, path } = window.nodeAPI;

let getFilepathMixin = (superClass) => class extends SettingsMixin(superClass) {

  async getFilepathAndIconpath(song, useStems, mvRestrict) {
    let filepath = '';
    let iconpath = '';
    if (mvRestrict && song.musicVideo && song.musicVideo.filePath) {
      filepath = path.join(this.settings.getString('dir.mv'), song.musicVideo.filePath);
      iconpath = path.join(this.settings.getString('dir.mv'), song.musicVideo.thumbnailFilePath);
    } else {
      filepath = path.isAbsolute(song.filepath) ? song.filepath : path.join(this.settings.getString('dir.music'), song.filepath);
      iconpath = song.album.coverFilepath ? (path.isAbsolute(song.album.coverFilepath) ? song.album.coverFilepath : path.join(this.settings.getString('dir.music'), song.album.coverFilepath)) : '';
      if (useStems && !this.settings.getBool('virtualdj.active') && !song.isTemporary()) {
        const stemRelPath = song.filepath.replace(/\.[^.]+$/, '.stem.m4a');
        const stemFilepath = path.join(this.settings.getString('dir.stems'), stemRelPath);
        try {
          const exists = await ipcRenderer.invoke('existsfilepath', stemFilepath);
          if (exists) {
            filepath = stemFilepath;
          }
        } catch(err) {
          this.addAlert("Failed checking for existence of stems file.\n" + err, 8);
        }
      }
    }
    return [filepath, iconpath]
  }
}

export const GetFilepathMixin = dedupingMixin(getFilepathMixin);
