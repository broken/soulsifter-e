const GooglePlayMusic = class {

  constructor() {
    let PlayMusic = require('playmusic');
    this.playMusic = new PlayMusic();
  }

  init(callback) {
    let settings = new ss.SoulSifterSettings();
    let email = settings.getString('google.email');
    let appKey = settings.getString('google.appKey');
    if (!email || !appKey) {
      console.error('No email and/or appKey for Google Music.')
      return;
    }
    this.playMusic.init({email: email, password: appKey}, (err) => {
      if(err) {
        console.error('Login to Google Music failed.');
        console.error(err);
      } else {
        console.info('Succesfully connected to Google Music.');
        callback();
      }
    });
  }

  /**
   * Add a new playlist, save the playlist id, and add the songs.
   */
  async createPlaylist(playlist) {
    this.init(() => {
      if (!playlist.gmusicId) {
        this.playMusic.addPlayList(playlist.name, function(err, mutationStatus) {
          if (err) {
            console.error('Unable to create playlist ' + playlist.name);
            console.error(err);
          } else {
            console.info('Successfully created playlist ' + playlist.name);
            playlist.gmusicId = mutationStatus.mutate_response[0].id;
            playlist.update();
            // todo add songs
          }
        });
      }
    });
  }

  /**
   * Callback returns err, streamUrl
   */
  async getStreamUrl(gmid) {
    return new Promise((resolve, reject) => {
      let callback = (err, url) => {
        if (err) reject(err);
        else resolve(url);
      }
      this.init(() => this.playMusic.getStreamUrl(gmid, callback));
    });
  }

  /**
   * Success callback takes a song.
   * Failure callback takes message, body, err, httpResonse
   * WARNING: Nothing uses this, and before usage, should verify it still works.
   */
  findSong(song, success, failure) {
    if (!this.connected) {
      return;
    }
    var maxResults = 3;
    this.playMusic.search(song.artist + ' ' + song.title, maxResults,
        function(err, data) {
          if (err) {
            console.error('Unable to search for song ' + song.artist + ' - ' + song.title);
            console.error(err);
            return;
          }
          console.log(data);
          if (!data.entries) {
            failure('No results in Google Play Music for ' + song.artist + ' - ' + song.title, '', 1);
            return;
          }
          // We first filter by type. Types are:
          // 1 : track result
          // 2 : artist result
          // 3 : album result
          // Then we take the result with the highest score.
          var track = data.entries
              .filter(function(val) { return val.type == 1; })
              .sort(function(a, b) { return a.score < b.score; })
              .shift();
          if (!track) {
            failure('No track results in Google Play Music for ' + song.artist + ' - ' + song.title, '', 2);
            return;
          }
          success(track);
        },
        function(message, body, err, httpResponse) {
          console.warn('Error searching Google Play Music for ' + song.artist + ' - ' + song.title + '.  ' + message + '\
' + body + '\
' + err + '\
' + httpResponse);
          failure(message, body, err, httpResonse);
        }
    );
  }
};

if (!window.gpm) window.gpm = new GooglePlayMusic();
