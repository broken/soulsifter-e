import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';

let getFilepathMixin = (superClass) => class extends superClass {

  async getFilepathAndIconpath(song, useStems, mvRestrict) {
    let filepath = '';
    let iconpath = '';
    if (mvRestrict) {
      filepath = this.settings.getString('dir.mv') + song.musicVideo.filePath;
      iconpath = this.settings.getString('dir.mv') + song.musicVideo.thumbnailFilePath;
    } else {
      filepath = this.settings.getString('dir.music') + song.filepath;
      iconpath = this.settings.getString('dir.music') + song.album.coverFilepath;
      if (useStems && !this.settings.getBool('virtualdj.active')) {
        let stemFilepath = this.settings.getString('dir.stems') + song.filepath.replace(/\.[^.]+$/, '.stem.m4a');
        try {
          let exists = await ipcRenderer.invoke('existsfilepath', stemFilepath);
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
