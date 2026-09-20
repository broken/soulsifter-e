import { dedupingMixin } from '@polymer/polymer/lib/utils/mixin.js';
import { AlertsMixin } from './mixin-alerts-pub.js';

let musicVideoMixin = (superClass) => class extends AlertsMixin(superClass) {

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

  async removeMusicVideo(song) {
    if (!song) return false;
    const songName = [song.artist, song.title].filter(Boolean).join(' - ') || song.id;
    const alertMsg = `Removing music video for ${songName}`;
    const alertId = this.addAlert(alertMsg, 0, -1);
    try {
      const res = ss.MusicVideoService.removeMusicVideo(song);
      song.musicVideo = null;
      let event = new CustomEvent('song-edit-changed', { detail: song.id });
      window.dispatchEvent(event);
      return res;
    } catch (err) {
      console.error('Failed to remove music video:', err);
      return false;
    } finally {
      this.rmAlert(alertId);
    }
  }

  // Returns true if we should proceed with selecting the song.
  async maybeEnsureMusicVideo(song, mvRestrict) {
    if (!song || !mvRestrict || !this.settings.getBool('mv.on_demand') || !this.settings.getBool('app.debug')) {
      return true;
    }
    if (song.musicVideo && song.musicVideo.filePath) {
      return true;
    }
    const query = [song.artist, song.title].filter(Boolean).join(' ');
    try {
      const res = await ipcRenderer.invoke('select-youtube-video', query, true);
      if (res.action === 'load-audio') {
        return true;
      }
      if (res.action === 'video' && res.url) {
        const mv = await this.associateMusicVideo(song, res.url);
        if (mv) {
          song.musicVideo = mv;
          let event = new CustomEvent('song-edit-changed', { detail: song.id });
          window.dispatchEvent(event);
        }
        return true;
      }
    } catch (err) {
      console.error('Failed to select or associate YouTube video:', err);
    }
    // res.action === 'cancel'
    return false;
  }
}

export const MusicVideoMixin = dedupingMixin(musicVideoMixin);
