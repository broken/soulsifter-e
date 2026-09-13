import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';
import { AlertsMixin } from './mixin-alerts-pub.js';

let getFilepathMixin = (superClass) => class extends AlertsMixin(superClass) {

  async getFilepathAndIconpath(song, useStems, mvRestrict) {
    let filepath = '';
    let iconpath = '';
    if (mvRestrict && song.musicVideo && song.musicVideo.filePath) {
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

  async associateMusicVideo(song, videoUrl) {
    if (!song || !videoUrl) return null;
    const songName = [song.artist, song.title].filter(Boolean).join(' - ') || song.id;
    const alertMsg = `Assigning music video for ${songName}`;
    const alertId = this.addAlert(alertMsg, 0, -1);
    try {
      const mv = await ss.MusicVideoService.associateYouTubeVideoAsync(song, videoUrl);
      return mv;
    } finally {
      this.rmAlert(alertId);
    }
  }

  async maybeEnsureMusicVideo(song, mvRestrict) {
    if (!song || !mvRestrict || !this.settings.getBool('mv.on_demand')) {
      return song;
    }
    if (song.musicVideo && song.musicVideo.filePath) {
      return song;
    }
    const query = [song.artist, song.title].filter(Boolean).join(' ');
    try {
      const videoUrl = await ipcRenderer.invoke('select-youtube-video', query);
      if (videoUrl) {
        const mv = await this.associateMusicVideo(song, videoUrl);
        if (mv) {
          song.musicVideo = mv;
          let event = new CustomEvent('song-edit-changed', { detail: song.id });
          window.dispatchEvent(event);
        }
      }
    } catch (err) {
      console.error('Failed to select or associate YouTube video:', err);
    }
    return song;
  }
}

export const GetFilepathMixin = dedupingMixin(getFilepathMixin);
