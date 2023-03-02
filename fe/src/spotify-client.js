import { AlertsMixin } from "./mixin-alerts.js";

class SpotifyClient extends AlertsMixin(Object) {  // TODO show errors

  constructor() {
    super();

    this.client_id = '557db31544664187ae1be4279d233166';
    this.redirect_uri = 'http://localhost:8888';
    this.logged_in = false;
    this.access_token = null;

    this.express = require('express'); // Express web server framework

    var SpotifyWebApi = require('spotify-web-api-node');
    // credentials are optional
    this.spotifyApi = new SpotifyWebApi({
      clientId: settings.getString('spotify.clientId'),
      clientSecret: settings.getString('spotify.clientSecret'),
      redirectUri: 'http://www.example.com/callback'
    });

    this._authorize();
  }

  /*
  I don't know what this code is. I just found it after a little hiatus, but spotify is working
  w/o calling it, so whatever. Yet, I hate this oauth stuff and I don't want to delete it in
  case it actually is useful, but I don't have the time to care atm since I'm not using spotify
  *
  _authorize_new() {
    // implicit grant flow
    let scopes = ['user-read-private', 'user-read-email'],
        redirectUri = 'http://localhost:8888',
        clientId = '557db31544664187ae1be4279d233166',
        showDialog = true,
        responseType = 'token',
        state = '';
    // generate state
    let possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    for (var i = 0; i < 16; i++) {
      state += possible.charAt(Math.floor(Math.random() * possible.length));
    }

    // Setting credentials can be done in the wrapper's constructor, or using the API object's setters.
    var SpotifyWebApi = require('spotify-web-api-node');
    this.spotifyApi = new SpotifyWebApi({
      redirectUri: redirectUri,
      clientId: clientId
    });

    // Create the authorization URL
    let authorizeURL = this.spotifyApi.createAuthorizeURL(
      scopes,
      state,
      showDialog,
      responseType
    );

    var request = require('request');
    let r = request.get(authorizeURL, (err, res, body) => {
      console.log("res.request.uri.href");
      console.log(res.request.uri.href);


      //spotifyApi.setAccessToken(code);
    });
  }*/

  _authorize() {
    // set up auth listener
    let app = this.express();
    app.get('/', (req, res) => { res.send('o hai') });
    console.log('Listening on 8888');
    this.server = app.listen(8888);

    // implicit grant flow
    let scope = 'playlist-modify-public user-read-private user-read-email';

    // generate random string
    this.state = '';
    let possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    for (var i = 0; i < 16; i++) {
      this.state += possible.charAt(Math.floor(Math.random() * possible.length));
    }

    let url = 'https://accounts.spotify.com/authorize';
    url += '?response_type=token';
    url += '&client_id=' + encodeURIComponent(this.client_id);
    url += '&scope=' + encodeURIComponent(scope);
    url += '&redirect_uri=' + encodeURIComponent(this.redirect_uri);
    url += '&state=' + encodeURIComponent(this.state);

    var out = {};
    nw.Window.open(url, {}, (win) => {
      // getHashParams
      let hashParams = {};
      let e, r = /([^&;=]+)=?([^&;]*)/g,
          q = win.window.location.hash.substring(1);
      while (e = r.exec(q)) {
         hashParams[e[1]] = decodeURIComponent(e[2]);
      }

      let access_token = hashParams.access_token,
          state = hashParams.state;
      if (access_token && (state == null || state !== this.state)) {
        alert('There was an error during the authentication');
      } else {
        if (access_token) {
          window.console.log('logged in!');
          this.spotifyApi.setAccessToken(access_token);
        } else {
          window.console.warn('not logged in');
        }
      }
      // shutdown everything only if the end page was loaded
      this.server.close();
      this.server = null;
      win.window.close();
    });
  }

  /** Callable Actions **/

  /**
   * Add a new playlist, save the playlist id, and add the songs.
   */
  async createPlaylist(playlistId) {
    let playlist = ss.Playlist.findById(playlistId);
    try {
      let response = await this.spotifyApi.createPlaylist(
          playlist.name,
          { 'description': 'SoulSifter playlist',
            'public': true });
      console.info('Successfully created Spotify playlist ' + playlist.name);
      playlist.spotifyId = response.body.id;
      playlist.update();
    } catch (err) {
      this.addAlert('Unable to create Spotify playlist ' + playlist.name);
      console.error(err);
      return;
    }
    if (!!playlist.query) {
      let songs = ss.SearchUtil.searchSongs(playlist.query, 0, '', playlist.styles, [], 200, 0, false, 0,
          (msg) => { this.addAlert(msg, 5); });
      let i = 0;
      try {
        for (i = 0; i < songs.length; ++i) {
          await this._addSongToPlaylist(songs[i], playlist);
        }
      } catch (err) {
        this.addAlert('Unable to add song ' + songs[i].id + ' to YouTube playlist ' + playlist.name);
        console.error(err);
        return;
      }
    }
  }

  async syncPlaylists() {
    try {
      // TODO add an alert for time.
      let playlists = ss.Playlist.findAll();
      for (let i = 0; i < playlists.length; ++i) {
        if (!playlists[i].spotifyId) continue;
        await this._updatePlaylistEntries(playlists[i]);
      }
    } catch (err) {
      this.addAlert('Failed to sync playlists.');
      console.error(err);
    }
  }

  /** Internal methods **/

  async _addSongToPlaylist(song, playlist) {
    if (!song.spotifyId) {
      let response = await this._findSong(song);
      if (!!response.body.tracks.total) {
        song.spotifyId = response.body.tracks.items[0].id;
        song.update();
      } else {
        console.warn('Unable to find song ' + song.id + ' [' + song.artist + ' - ' + song.title + '] in Spotify');
      }
    }
    if (!!song.spotifyId) {
      await this.spotifyApi.addTracksToPlaylist(playlist.spotifyId, ['spotify:track:' + song.spotifyId]);
    }
  }

  _findSong(song) {
    return this.spotifyApi.searchTracks('track:' + song.title + ' artist:' + song.artist, {'limit': 1});
  }

  async _updatePlaylistEntries(playlist) {
    let entries, songs;
    if (!playlist.query) {
      entries = ss.PlaylistEntry.findByPlaylistId(this.playlist.id);
      songs = entries.sort(function(a, b) { return a.position - b.position; })
                     .map(function(x) { return x.song; });
    } else {
      entries = [];
      songs = ss.SearchUtil.searchSongs(playlist.query + ' trashed:0', 0, '', playlist.styles, [], 200);  // todo: err callbck
    }

    // get youtube playlist items
    /*let items = [];
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
    }*/

    // add songs
    try {
      for (let i = 0; i < songs.length; ++i) {
        await this._addSongToPlaylist(songs[i], playlist);
      }
    } catch (err) {
      this.addAlert('Unable to add song ' + songs[i].id + ' to YouTube playlist ' + playlist.name);
      throw err;
    }
  }

};

if (!window.spotify) window.spotify = new SpotifyClient();
