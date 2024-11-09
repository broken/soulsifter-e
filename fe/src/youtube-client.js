class YoutubeClient extends Object {

  createPlaylist(playlistId) {
    ipcRenderer.send('yt-createPlaylist', playlistId);
  }

  addPlaylistEntry(entryId) {
    ipcRenderer.send('yt-addPlaylistEntry', entryId);
  }

  deletePlaylist(ytPlaylistId) {
    ipcRenderer.send('yt-deletePlaylist', ytPlaylistId);
  }

  syncPlaylists(alertId) {
    ipcRenderer.send('yt-syncPlaylists', alertId);
  }

  updatePlaylistEntries(playlistId, alertId) {
    ipcRenderer.send('yt-updatePlaylistEntries', playlistId, alertId);
  }

};

if (!window.yt) window.yt = new YoutubeClient();
