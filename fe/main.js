// Modules to control application life and create native browser window
const { app, clipboard, dialog, BrowserWindow, ipcMain, nativeImage } = require('electron')
const path = require('path');
const { google } = require('googleapis');
const ss = require('../be');

const isDev = process.env.IS_DEV === 'true';

const createWindow = () => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1680,
    height: 498,
    x: 0,
    y: 0,
    icon: path.join(__dirname, 'DVDRipper.icns'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('build/index.html')

  // Open the DevTools.
  mainWindow.webContents.openDevTools()
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()

  app.on('activate', () => {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

ipcMain.on('ondragstart', (event, filepath, icon) => {
  nativeImage.createThumbnailFromPath(icon, {width: 50, height: 50})
  .then((img) => {
    event.sender.startDrag({
      file: filepath,
      icon: img,
    });
  })
  .catch((err) => {
    console.log(err);
  });
})

ipcMain.handle('opendialog', async (event, title, defaultPath, props) => {
  const result = await dialog.showOpenDialog({
    title: title,
    defaultPath: defaultPath,
    properties: props
  });
  return result;
})

ipcMain.handle('getclipboard', (event) => {
  return clipboard.readText();
})

class YoutubeClientMain {

  constructor() {
    this.service = null;
    this.oauth2Client = null;
  }

  sleep(ms=500) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  auth() {
    return new Promise((resolve, reject) => {
      if (!this.service) {
        let settings = new ss.SoulSifterSettings();
        this.oauth2Client = new google.auth.OAuth2(
            settings.getString('google.clientId'),
            settings.getString('google.clientSecret'),
            'http://localhost:3000/oauth2callback'
        );
        google.options({auth: this.oauth2Client});
        this.service = google.youtube('v3');
        // const refreshToken = settings.getString('google.oauthRefreshToken');
        // if (!!refreshToken) {
        //   this.oauth2Client.setCredentials({ refresh_token: refreshToken });
        //   return new Promise((resolve, reject) => {
        //     resolve(this.oauth2Client);
        //   });
        // }
        const authUrl = this.oauth2Client.generateAuthUrl({
          access_type: 'offline',
          scope: ['https://www.googleapis.com/auth/youtube'],
        });
        const authWindow = new BrowserWindow({
          width: 800,
          height: 600,
          show: false,
          'node-integration': false,
          webPreferences: {
            nodeIntegration: false,
          },
        });
        authWindow.loadURL(authUrl);
        authWindow.show();
        authWindow.on('closed', () => {
          // Handle the window being closed
          console.log('authwindow closed');
        });
        authWindow.webContents.on('will-redirect', (event, url) => {
          console.log('capture will redirect: ' + url);
          if (url.startsWith(this.oauth2Client.redirectUri)) {
            let q = require('url').parse(url, true).query;
            console.log('code: ' + q.code);
            this.oauth2Client.getToken(q.code, (err, tokens) => {
              console.log('err: ' + err);
              console.log('tokens; ', tokens);
              if (!err) {
                this.oauth2Client.setCredentials(tokens);
                // Store the access and refresh tokens for future use
                // tokens.access_token
                // tokens.refresh_token
                // tokens.expiry_date
                resolve();
              } else {
                reject(err);
              }
              authWindow.destroy();
            });
          }
        });
      } else {
        resolve();
      }
    });
  }

  /**
   * This method opens a new window to let users log-in the OAuth provider service,
   * grant permissions to OAuth client service (this application),
   * and returns OAuth code which can be exchanged for the real API access keys.
   *
   * @param {*} interactionWindow a window in which the user will have interaction with OAuth provider service.
   * @param {*} authPageURL an URL of OAuth provider service, which will ask the user grants permission to us.
   * @returns {Promise<string>}
   */
  // const getOAuthCodeByInteraction = (interactionWindow, authPageURL) => {
  //   interactionWindow.loadURL(authPageURL);
  //   return new Promise((resolve, reject) => {
  //       const onclosed = () => {
  //           reject('Interaction ended intentionally ;(');
  //       };
  //       interactionWindow.on('closed', onclosed);
  //       interactionWindow.on('page-title-updated', (ev) => {
  //           const url = new URL(ev.sender.getURL());
  //           if (url.searchParams.get('approvalCode')) {
  //               interactionWindow.removeListener('closed', onclosed);
  //               interactionWindow.close();
  //               return resolve(url.searchParams.get('approvalCode'));
  //           }
  //           if ((url.searchParams.get('response') || '').startsWith('error=')) {
  //               interactionWindow.removeListener('closed', onclosed);
  //               interactionWindow.close();
  //               return reject(url.searchParams.get('response'));
  //           }
  //       });
  //   });
  // };
  // /**
  //  * Add a new playlist, save the playlist id, and add the songs.
  //  */
  // async createPlaylist(playlistId) {
  //   if (!this._auth()) return;
  //   let playlist = ss.Playlist.findById(playlistId);
  //   if (!playlist.youtubeId) {
  //     try {
  //       let response = await this._createPlaylist(playlist);
  //       console.info('Successfully created YouTube playlist ' + playlist.name);
  //       playlist.youtubeId = response.data.id;
  //       playlist.update();
  //     } catch (err) {
  //       this.addAlert('Unable to create YouTube playlist ' + playlist.name);
  //       console.error(err);
  //       if (err.message == 'invalid_grant') {
  //         this.service = undefined;
  //         const settings = new ss.SoulSifterSettings();
  //         settings.putString('google.oauthRefreshToken', '');
  //         settings.save();
  //       }
  //       return;
  //     }
  //   }
  //   try {
  //     await this._updatePlaylistEntries(playlist);
  //     // if (!!playlist.query) {
  //     //   let songs = ss.SearchUtil.searchSongs(playlist.query,
  //     //                                         /* bpm */ 0,
  //     //                                         /* key */ '',
  //     //                                         playlist.styles,
  //     //                                         /* songsToOmit */ [],
  //     //                                         /* limit */ 200,
  //     //                                         /* energy */ 0,
  //     //                                         /* musicVideoMode */ false,
  //     //                                         /* orderBy */ 0,
  //     //                                         /* errorCallack */ (msg) => { this.addAlert(msg, 5); });
  //     //   for (let i = 0; i < songs.length; ++i) {
  //     //     await this._addSongToPlaylist(songs[i], playlist);
  //     //     await this.sleep();
  //     //   }
  //     // }
  //   } catch (err) {
  //     this.addAlert('Unable to add song to YouTube playlist ' + playlist.name);
  //     console.error(err);
  //     return;
  //   }
  // }

  async _addSongToPlaylist(song, playlist) {
    if (!song.youtubeId) {
      let response = await this._findSong(song);
      if (!response.data.items.length) {
        console.warn('Unable to find song ' + song.id + ' : ' + song.artist + ' - ' + song.title);
        return Promise.resolve();
      }
      song.youtubeId = response.data.items[0].id.videoId;
      song.update();
    }
    await this._addPlaylistItem(playlist.youtubeId, song.youtubeId);
  }

  // async addPlaylistEntry(entryId) {
  //   if (!this._auth()) return;
  //   let entry = ss.PlaylistEntry.findById(entryId);
  //   try {
  //     if (!entry.song.youtubeId) await this.syncSong(entry.song);
  //     await this._addPlaylistEntry(entry);
  //   } catch (err) {
  //     this.addAlert('Unable to add playlist entry ' + entryId);
  //     console.error(err);
  //   }
  // }

  // async syncSong(song) {
  //   if (!this._auth()) return;
  //   let response = await this._findSong(song);
  //   if (!response.data.items.length) {
  //     console.warn('Unable to find song ' + song.id + ' : ' + song.artist + ' - ' + song.title);
  //     return Promise.resolve();
  //   }
  //   song.youtubeId = response.data.items[0].id.videoId;
  //   song.update();
  //   return Promise.resolve();
  // }

  // async deletePlaylist(ytPlaylistId) {
  //   if (!this._auth()) return;
  //   try {
  //     await this._deletePlaylist(ytPlaylistId);
  //   } catch (err) {
  //     this.addAlert('Unable to delete YouTube playlist ' + ytPlaylistId);
  //     console.error(err);
  //   }
  // }

  // async syncPlaylists() {
  //   if (!this._auth()) return;
  //   let alertId = this.addAlert('Syncing playlists.', 0, -1);
  //   let progress = 0;
  //   try {
  //     // TODO add an alert for time.
  //     let playlists = ss.Playlist.findAll();
  //     for (let i = 0; i < playlists.length; ++i) {
  //       if (!playlists[i].youtubeId) continue;
  //       progress = i / playlists.length || 0.01;
  //       this.updateAlert(alertId, progress, 'Syncing playlist ' + playlists[i].name);
  //       await this._updatePlaylistEntries(playlists[i]);
  //     }
  //     this.updateAlert(alertId, 1, 'Completed syncing playlists.', 20);
  //   } catch (err) {
  //     this.updateAlert(alertId, progress, 'Failed to sync playlists. ' + err);
  //     console.error(err);
  //   }
  // }

  async _updatePlaylistEntries(playlist) {
    let entries, songs;
    if (!playlist.query) {
      entries = ss.PlaylistEntry.findByPlaylistId(playlist.id);
      songs = entries.sort(function(a, b) { return a.position - b.position; })
                    .map(function(x) { return x.song; });
    } else {
      songs = ss.SearchUtil.searchSongs(playlist.query + ' trashed:0',
                                        /* bpm */ 0,
                                        /* key */ '',
                                        playlist.styles,
                                        /* songsToOmit */ [],
                                        /* limit */ 200,
                                        /* energy */ 0,
                                        /* musicVideoMode */ false,
                                        /* orderBy */ 0,
                                        /* errorCallack */ (msg) => { console.error(msg); });
    }

    // get youtube playlist items
    let items = [];
    let pageToken = undefined;
    do {
      let response = await this._getPlaylistItems(playlist, pageToken);
      for (let i = 0; i < response.data.items.length; ++i) {
        let item = response.data.items[i];
        items.push({
          id: item.id,
          videoId: item.snippet.resourceId.videoId,
          position: item.snippet.position
        });
      }
      pageToken = response.data.nextPageToken;
    } while (!!pageToken);

    // filter out dupes with songs
    let x = items.filter(item => !songs.find(s => s.youtubeId == item.videoId));
    songs = songs.filter(song => !items.find(i => song.youtubeId == i.videoId));
    items = x;

    // remove items
    for (let i = 0; i < items.length; ++i) {
      await this._removePlaylistEntry(items[i].id);
      await this.sleep();
    }

    // add songs
    for (let i = 0; i < songs.length; ++i) {
      await this._addSongToPlaylist(songs[i], playlist);
      await this.sleep();
    }
  }

  // _createPlaylist(playlist) {
  //   return new Promise((resolve, reject) => {
  //     console.info('Creating playlist ' + playlist.name);
  //     this.service.playlists.insert({
  //       auth: this.oauth2Client,
  //       part: 'snippet,status',
  //       resource: {
  //         snippet: {
  //           title: playlist.name,
  //           description: 'managed by SoulSifter'
  //         },
  //         status: {
  //           privacyStatus: 'private'
  //         }
  //       }
  //     }, (err, response) => {
  //       if (err) reject(err);
  //       else resolve(response);
  //     });
  //   });
  // }

  _getPlaylistItems(playlist, nextPageToken=undefined) {
    let request = {
        auth: this.oauth2Client,
        part: 'contentDetails,id,snippet,status',
        playlistId: playlist.youtubeId,
        maxResults: 20
      };
    if (!!nextPageToken) request.pageToken = nextPageToken;
    return new Promise((resolve, reject) => {
      console.info('Getting playlist items.');
      this.service.playlistItems.list(request, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }

  _findSong(song) {
    return new Promise((resolve, reject) => {
      console.info('Looking up song ' + song.id + ' on YouTube.');
      this.service.search.list({
        auth: this.oauth2Client,
        part: 'snippet',
        q: song.artist + ' ' + song.title,
        type: 'video',
        topicId: '/m/04rlf',
        maxResults: 1
      }, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }

  _addPlaylistItem(playlistYoutubeId, songYoutubeId, position = -1) {
    return new Promise((resolve, reject) => {
      console.info('Adding song ' + songYoutubeId + ' to playlist ' + playlistYoutubeId);
      this.service.playlistItems.insert({
        auth: this.oauth2Client,
        part: 'snippet',
        resource: {
          snippet: {
            playlistId: playlistYoutubeId,
            resourceId: {
              kind: 'youtube#video',
              videoId: songYoutubeId
            },
            ...((position>0) && { position: position }),
          }
        }
      }, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }

  // _addPlaylistEntry(entry) {
  //   return _addPlaylistItem(entry.playlist.youtubeId, entry.song.youtubeId, entry.position);
  // }

  _removePlaylistEntry(itemId) {
    return new Promise((resolve, reject) => {
      console.info('Deleting playlist item ' + itemId);
      this.service.playlistItems.delete({
        auth: this.oauth2Client,
        id: itemId
      }, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }

  // _deletePlaylist(playlistId) {
  //   return new Promise((resolve, reject) => {
  //     console.info('Deleting playlist ' + playlistId);
  //     this.service.playlists.delete({
  //       auth: this.oauth2Client,
  //       id: playlistId
  //     }, (err, response) => {
  //       if (err) reject(err);
  //       else resolve(response);
  //     });
  //   });
  // }
};
let yt = new YoutubeClientMain();

ipcMain.on('yt-updatePlaylistEntries', async (event, playlistId) => {
  yt.auth().then(async () => {
    let playlist = ss.Playlist.findById(playlistId);
    await yt._updatePlaylistEntries(playlist);
  }).catch(err => {
    console.error('Unable to update playlist entries for playlist ' + playlistId);
    console.error(err);
  });
});

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
