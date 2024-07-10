// Modules to control application life and create native browser window
const { app, clipboard, dialog, BrowserWindow, ipcMain, nativeImage } = require('electron');
const fs = require('fs');
const path = require('path');
const { google } = require('googleapis');
const ss = require('soulsifter');

const isDev = process.env.IS_DEV === 'true';

const createWindow = () => {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 1680,
    height: 530,
    x: 0,
    y: 0,
    icon: path.join(__dirname, 'soulsifter.icns'),
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      nodeIntegration: true,
      contextIsolation: false
    }
  })

  // and load the index.html of the app.
  mainWindow.loadFile('build/index.html')

  // Open the DevTools.
  if (!app.isPackaged) {
    mainWindow.webContents.openDevTools();
  }

  ipcMain.on('opendevtools', (event) => {
    mainWindow.webContents.openDevTools();
  });

  // Enable MIDI permissions
  mainWindow.webContents.session.setPermissionRequestHandler((webContents, permission, callback, details) => {
    if (permission === 'midi' || permission === 'midiSysex') {
      callback(true);
    } else {
      callback(false);
    }
  })

  mainWindow.webContents.session.setPermissionCheckHandler((webContents, permission, requestingOrigin) => {
    if (permission === 'midi' || permission === 'midiSysex') {
      return true;
    }
    return false;
  });

  ipcMain.on('yt-syncPlaylists', async (event) => {
    yt.auth().then(async () => {
      // let alertId = this.addAlert('Syncing playlists.', 0, -1); TODO
      let progress = 0;
      // TODO add an alert for time.
      let playlists = ss.Playlist.findAll();
      for (let i = 0; i < playlists.length; ++i) {
        if (!playlists[i].youtubeId) continue;
        progress = i / playlists.length || 0.01;
        // this.updateAlert(alertId, progress, 'Syncing playlist ' + playlists[i].name);
        await yt._updatePlaylistEntries(playlists[i]);
      }
      // this.updateAlert(alertId, 1, 'Completed syncing playlists.', 20);
    }).catch(err => {
      console.error('Failed to sync playlists: ' + err);
      mainWindow.webContents.send('addalert', {'progress': progress, 'a': 'Failed to sync playlists. ' + err});
      // this.updateAlert(alertId, progress, 'Failed to sync playlists. ' + err);
      console.error(err);
    });
  });

  ipcMain.on('yt-createPlaylist', async (event, playlistId) => {
    yt.auth().then(async () => {
      let playlist = ss.Playlist.findById(playlistId);
      if (!playlist.youtubeId) {
        let response = await yt._createPlaylist(playlist);
        console.info('Successfully created YouTube playlist ' + playlist.name);
        playlist.youtubeId = response.data.id;
        playlist.update();
      }
      await yt._updatePlaylistEntries(playlist);
    }).catch(err => {
      mainWindow.webContents.send('addalert', {'a': 'Unable to create YouTube playlist ' + playlist.name});
      // this.addAlert('Unable to create YouTube playlist ' + playlist.name);
      // if (err.message == 'invalid_grant') {
      //   this.service = undefined;
      //   const settings = new ss.SoulSifterSettings();
      //   settings.putString('google.oauthRefreshToken', '');
      //   settings.save();
      // }
      // return;
      console.error('Unable to create YouTube playlist ' + playlist.name);
      console.error(err);
    });
  });
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

var ss_img = nativeImage.createFromPath(path.join(__dirname, 'resources/soulsifter.png')).resize({width: 50, quality: 'best'});
ipcMain.on('ondragstart', (event, filepath, icon) => {
  let ext = icon.split('.').pop().toLowerCase();
  let img = ss_img;
  if (ext == 'jpg' || ext == 'jpeg' || ext == 'png') {
    let tmp = nativeImage.createFromPath(icon);
    if (!tmp.isEmpty()) {
      img = tmp;
      let size = img.getSize();
      size.x = 0, size.y = 0;
      if (size.height > size.width) {
        size.y = Math.floor((size.height - size.width) / 2);
        size.height = size.width;
      } else if (size.width > size.height) {
        size.x = Math.floor((size.width - size.height) / 2);
        size.width = size.height;
      }
      console.log(size);
      img = img.crop(size).resize({width: 50, quality: 'good'});
    }
  }
  event.sender.startDrag({
    file: filepath,
    icon: img,
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

ipcMain.handle('existsfilepath', (event, fp) => {
  return fs.existsSync(fp);
})

ipcMain.handle('getclipboard', (event) => {
  return clipboard.readText();
})

ipcMain.on('copytoclipboard', (event, str) => {
  clipboard.writeText(str);
})

class YoutubeClientMain {

  constructor() {
    this.service = google.youtube('v3');
    this.oauth2Client = null;
    this.expiry_date = 0;
  }

  sleep(ms=1000) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  isExpired(bufferInMins=2) {
    return new Date().getTime() + bufferInMins * 60 * 1000 >= this.expiry_date;
  }

  async auth(reauthenticate=false) {
    // check if already authenticated
    if (!!this.oauth2Client && !reauthenticate && !this.isExpired()) return;

    // oauth object
    let settings = new ss.SoulSifterSettings();
    this.oauth2Client = new google.auth.OAuth2(
        settings.getString('google.clientId'),
        settings.getString('google.clientSecret'),
        'http://localhost:3000/oauth2callback'
    );
    google.options({auth: this.oauth2Client});

    // use access token if available
    if (!reauthenticate) {
      const accessToken = settings.getString('google.oauthAccessToken');
      if (!!accessToken) {
        this.expiry_date = Number(settings.getString('google.oauthAccessTokenExpiration'));
        if (!this.isExpired()) {
          this.oauth2Client.setCredentials({ access_token: accessToken });
          console.log('Access token used.');
          return;
        } else console.log('access token is expired');
      } else console.log('no access token to use');
    } else console.log('reauthenticating');

    // attempt to get new access token from the refresh token
    const refreshToken = settings.getString('google.oauthRefreshToken');
    if (!!refreshToken) {
      this.oauth2Client.setCredentials({ refresh_token: refreshToken });
      try {
        const tokens = await this.oauth2Client.refreshAccessToken();
        console.log('Refresh token used to generate new access token.');
        console.log('tokens: ', tokens);
        this.oauth2Client.setCredentials(tokens.credentials);
        settings.putString('google.oauthAccessToken', tokens.credentials.access_token);
        settings.putString('google.oauthAccessTokenExpiration', tokens.credentials.expiry_date.toString());
        settings.putString('google.oauthRefreshToken', tokens.credentials.refresh_token);
        this.expiry_date = tokens.credentials.expiry_date;
        settings.save();
        return;
      } catch (error) {
        // refresh token expired or revoked. continue with flow.
        console.log('Unable to refresh access token. ', error);
      }
    } else console.log('no refresh token available.');

    // authenticate with user
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
      // TODO Handle the window being closed
      console.log('authwindow closed');
    });
    const result = await new Promise((resolve, reject) => {
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
              settings.putString('google.oauthAccessToken', tokens.access_token);
              settings.putString('google.oauthRefreshToken', tokens.refresh_token);
              // write as string since int doesn't seem to be big enough
              settings.putString('google.oauthAccessTokenExpiration', tokens.expiry_date.toString());
              this.expiry_date = tokens.expiry_date;
              settings.save();
              resolve();
            } else {
              console.warn('throwing error: ', err);
              reject(err);
            }
            authWindow.destroy();
          });
        }
      });
    });
  }

  async _addSongToPlaylist(song, playlist) {
    if (!song.youtubeId) {
      let response = await this._findSong(song);
      if (!response.data.items.length) {
        console.warn('Unable to find song ' + song.id + ' : ' + song.artist + ' - ' + song.title);
        return Promise.resolve();
      }
      song.youtubeId = response.data.items[0].id.videoId;
      song.update();
      await this.sleep();
    }
    await this._addPlaylistItem(playlist.youtubeId, song.youtubeId);
  }

  async syncSong(song) {
    let response = await this._findSong(song);
    if (!response.data.items.length) {
      console.warn('Unable to find song ' + song.id + ' : ' + song.artist + ' - ' + song.title);
      return Promise.reject();
    }
    song.youtubeId = response.data.items[0].id.videoId;
    song.update();
    return Promise.resolve();
  }

  async _updatePlaylistEntries(playlist) {
    let entries, songs;
    if (!playlist.query) {
      entries = ss.PlaylistEntry.findByPlaylistId(playlist.id);
      songs = entries.sort(function(a, b) { return a.position - b.position; })
                    .map(function(x) { return x.song; });
    } else {
      songs = ss.SearchUtil.searchSongs(playlist.query + ' trashed:0',
                                        /* limit */ 200,
                                        /* bpm */ 0,
                                        /* key */ '',
                                        playlist.styles,
                                        /* songsToOmit */ [],
                                        /* playlists */ [],
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

  _createPlaylist(playlist) {
    return new Promise((resolve, reject) => {
      console.info('Creating playlist ' + playlist.name);
      this.service.playlists.insert({
        auth: this.oauth2Client,
        part: 'snippet,status',
        resource: {
          snippet: {
            title: playlist.name,
            description: 'managed by SoulSifter'
          },
          status: {
            privacyStatus: 'private'
          }
        }
      }, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }

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

  _addPlaylistEntry(entry) {
    return _addPlaylistItem(entry.playlist.youtubeId, entry.song.youtubeId, entry.position);
  }

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

  _deletePlaylist(playlistId) {
    return new Promise((resolve, reject) => {
      console.info('Deleting playlist ' + playlistId);
      this.service.playlists.delete({
        auth: this.oauth2Client,
        id: playlistId
      }, (err, response) => {
        if (err) reject(err);
        else resolve(response);
      });
    });
  }
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

ipcMain.on('yt-deletePlaylist', async (event, ytPlaylistId) => {
  yt.auth().then(async () => {
    await yt._deletePlaylist(ytPlaylistId);
  }).catch(err => {
    console.error('Unable to delete YouTube playlist ' + ytPlaylistId);
    console.error(err);
  });
});

ipcMain.on('yt-addPlaylistEntry', async (event, entryId) => {
  yt.auth().then(async () => {
    let entry = ss.PlaylistEntry.findById(entryId);
    if (!entry.song.youtubeId) await yt.syncSong(entry.song);
    await yt._addPlaylistEntry(entry);
  }).catch(err => {
    console.error('Unable to add playlist entry ' + entryId);
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
