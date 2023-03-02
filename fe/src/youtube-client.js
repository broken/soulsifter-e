import { AlertsMixin } from "./mixin-alerts.js";

class YoutubeClient extends AlertsMixin(Object) {  // TODO show errors

  async _oauth() {
    const fs = require('fs');
    const path = require('path');
    const http = require('http');
    const url = require('url');
    const opn = require('open');
    const destroyer = require('server-destroy');

    const {google} = require('googleapis');

    const settings = new ss.SoulSifterSettings();

    const oauth2Client = new google.auth.OAuth2(
      settings.getString('google.clientId'),
      settings.getString('google.clientSecret'),
      'http://localhost:3000/oauth2callback'
    );
    google.options({auth: oauth2Client});
    this.service = google.youtube('v3');

    const refreshToken = settings.getString('google.oauthRefreshToken');
    if (!!refreshToken) {
      oauth2Client.setCredentials({ refresh_token: refreshToken });
      return new Promise((resolve, reject) => {
        resolve(oauth2Client);
      });
    }

    /**
     * Open an http server to accept the oauth callback. In this simple example, the only request to our webserver is to /callback?code=<code>
     */
    async function authenticate() {
      return new Promise((resolve, reject) => {
        // grab the url that will be used for authorization
        const authorizeUrl = oauth2Client.generateAuthUrl({
          // 'online' (default) or 'offline' (gets refresh_token)
          access_type: 'offline',
          prompt: 'consent',  // force consent to get refresh token
          scope: 'https://www.googleapis.com/auth/youtube',
        });
        const server = http.createServer(async (req, res) => {
          try {
            if (req.url.indexOf('/oauth2callback') > -1) {
              const qs = new url.URL(req.url, 'http://localhost:3000').searchParams;
              res.end('Authentication successful! Please return to the console.');
              server.destroy();
              const {tokens} = await oauth2Client.getToken(qs.get('code'));
              oauth2Client.credentials = tokens;
              if (!!tokens.refresh_token) {
                // store the refresh_token
                settings.putString('google.oauthRefreshToken', tokens.refresh_token);
                settings.save();
              }
              resolve(oauth2Client);
            }
          } catch (e) {
            reject(e);
          }
        }).listen(3000, () => {
          // open the browser to the authorize url to start the workflow
          opn(authorizeUrl, {wait: false}).then(cp => cp.unref());
        });
        destroyer(server);
      });
    }

    // Authorize a client with the loaded credentials, then call the YouTube API.
    return authenticate();
  }

  async _auth() {
    if (!this.service) {
      try {
        await this._oauth();
      } catch(err) {
        this.addAlert('Unable authenticate');
        console.error(err);
        return false;
      }
    }
    return true;
  }

  sleep(ms=500) {
    return new Promise(resolve => setTimeout(resolve, ms));
  }

  /**
   * Add a new playlist, save the playlist id, and add the songs.
   */
  async createPlaylist(playlistId) {
    if (!this._auth()) return;
    let playlist = ss.Playlist.findById(playlistId);
    if (!playlist.youtubeId) {
      try {
        let response = await this._createPlaylist(playlist);
        console.info('Successfully created YouTube playlist ' + playlist.name);
        playlist.youtubeId = response.data.id;
        playlist.update();
      } catch (err) {
        this.addAlert('Unable to create YouTube playlist ' + playlist.name);
        console.error(err);
        if (err.message == 'invalid_grant') {
          this.service = undefined;
          const settings = new ss.SoulSifterSettings();
          settings.putString('google.oauthRefreshToken', '');
          settings.save();
        }
        return;
      }
    }
    try {
      await this._updatePlaylistEntries(playlist);
      // if (!!playlist.query) {
      //   let songs = ss.SearchUtil.searchSongs(playlist.query,
      //                                         /* bpm */ 0,
      //                                         /* key */ '',
      //                                         playlist.styles,
      //                                         /* songsToOmit */ [],
      //                                         /* limit */ 200,
      //                                         /* energy */ 0,
      //                                         /* musicVideoMode */ false,
      //                                         /* orderBy */ 0,
      //                                         /* errorCallack */ (msg) => { this.addAlert(msg, 5); });
      //   for (let i = 0; i < songs.length; ++i) {
      //     await this._addSongToPlaylist(songs[i], playlist);
      //     await this.sleep();
      //   }
      // }
    } catch (err) {
      this.addAlert('Unable to add song to YouTube playlist ' + playlist.name);
      console.error(err);
      return;
    }
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
    }
    await this._addPlaylistItem(playlist.youtubeId, song.youtubeId);
  }

  async addPlaylistEntry(entryId) {
    if (!this._auth()) return;
    let entry = ss.PlaylistEntry.findById(entryId);
    try {
      if (!entry.song.youtubeId) await this.syncSong(entry.song);
      await this._addPlaylistEntry(entry);
    } catch (err) {
      this.addAlert('Unable to add playlist entry ' + entryId);
      console.error(err);
    }
  }

  async syncSong(song) {
    if (!this._auth()) return;
    let response = await this._findSong(song);
    if (!response.data.items.length) {
      console.warn('Unable to find song ' + song.id + ' : ' + song.artist + ' - ' + song.title);
      return Promise.resolve();
    }
    song.youtubeId = response.data.items[0].id.videoId;
    song.update();
    return Promise.resolve();
  }

  async deletePlaylist(ytPlaylistId) {
    if (!this._auth()) return;
    try {
      await this._deletePlaylist(ytPlaylistId);
    } catch (err) {
      this.addAlert('Unable to delete YouTube playlist ' + ytPlaylistId);
      console.error(err);
    }
  }

  async syncPlaylists() {
    if (!this._auth()) return;
    let alertId = this.addAlert('Syncing playlists.', 0, -1);
    let progress = 0;
    try {
      // TODO add an alert for time.
      let playlists = ss.Playlist.findAll();
      for (let i = 0; i < playlists.length; ++i) {
        if (!playlists[i].youtubeId) continue;
        progress = i / playlists.length || 0.01;
        this.updateAlert(alertId, progress, 'Syncing playlist ' + playlists[i].name);
        await this._updatePlaylistEntries(playlists[i]);
      }
      this.updateAlert(alertId, 1, 'Completed syncing playlists.', 20);
    } catch (err) {
      this.updateAlert(alertId, progress, 'Failed to sync playlists. ' + err);
      console.error(err);
    }
  }

  async updatePlaylistEntries(playlistId) {
    if (!this._auth()) return;
    try {
      // get playlist songs
      let playlist = ss.Playlist.findById(playlistId);
      await this._updatePlaylistEntries(playlist);
    } catch (err) {
      this.addAlert('Unable to update playlist entries for playlist ' + playlistId);
      console.error(err);
    }
  }

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
                                        /* errorCallack */ (msg) => { this.addAlert(msg, 5); });
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

if (!window.yt) window.yt = new YoutubeClient();
