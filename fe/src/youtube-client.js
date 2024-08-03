import { AlertsMixin } from "./mixin-alerts-pub.js";

class YoutubeClient extends AlertsMixin(Object) {  // TODO show errors

  /**
   * Add a new playlist, save the playlist id, and add the songs.
   */
  createPlaylist(playlistId) {
    ipcRenderer.send('yt-createPlaylist', playlistId);
  }

  addPlaylistEntry(entryId) {
    ipcRenderer.send('yt-addPlaylistEntry', entryId);
  }

  deletePlaylist(ytPlaylistId) {
    ipcRenderer.send('yt-deletePlaylist', ytPlaylistId);
  }

  syncPlaylists() {
    ipcRenderer.send('yt-syncPlaylists');
  }

  updatePlaylistEntries(playlistId, alertId) {
    ipcRenderer.send('yt-updatePlaylistEntries', playlistId, alertId);
  }

};

if (!window.yt) window.yt = new YoutubeClient();
