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

  async maybeEnsureMusicVideo(song, mvRestrict) {
    if (!song || !mvRestrict || !this.settings.getBool('mv.on_demand') || !this.settings.getBool('app.debug')) {
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

export const MusicVideoMixin = dedupingMixin(musicVideoMixin);
