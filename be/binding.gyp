{
  "targets": [
    {
      "target_name": "soulsifter",
      "sources": [
        # module
        "src/soulsifter.cc",
        # model
        "src/Album.cpp",
        "src/Album_wrap.cpp",
        "src/AlbumHelper.cpp",
        "src/AlbumPart.cpp",
        "src/AlbumPart_wrap.cpp",
        "src/BasicGenre.cpp",
        "src/BasicGenre_wrap.cpp",
        "src/BasicGenreHelper.cpp",
        "src/Mix.cpp",
        "src/Mix_wrap.cpp",
        "src/MixHelper.cpp",
        "src/MusicVideo.cpp",
        "src/MusicVideo_wrap.cpp",
        "src/Playlist.cpp",
        "src/Playlist_wrap.cpp",
        "src/PlaylistEntry.cpp",
        "src/PlaylistEntry_wrap.cpp",
        "src/REAlbumCover.cpp",
        "src/RESetting.cpp",
        "src/RESettingHelper.cpp",
        "src/RESong.cpp",
        "src/Song.cpp",
        "src/Song_wrap.cpp",
        "src/SongHelper.cpp",
        "src/Style.cpp",
        "src/Style_wrap.cpp",
        "src/StyleHelper.cpp",
        # detection
        "src/AudioAnalyzer.cpp",
        "src/AudioAnalyzer_wrap.cpp",
        "src/bstdfile.c",
        "src/madlld.cpp",
        "src/MiniBpm.cpp",
        # organization
        "src/FilesToAdd.cpp",
        "src/MusicManager.cpp",
        "src/MusicService.cpp",
        "src/MusicService_wrap.cpp",
        "src/MusicVideoService.cpp",
        "src/MusicVideoService_wrap.cpp",
        "src/NewSongManager.cpp",
        "src/NewSongManager_wrap.cpp",
        "src/SearchUtil.cpp",
        "src/SearchUtil_wrap.cpp",
        "src/SoulSifterSettings.cpp",
        "src/SoulSifterSettings_wrap.cpp",
        "src/TagService.cpp",
        "src/TagService_wrap.cpp",
        # persistence
        "src/MysqlAccess.h",
        "src/MysqlAccess.cpp",
        "src/ResultSetIterator.h",
        # util
        "src/AlertsChannel.cpp",
        "src/AlertsChannel_wrap.cpp",
        "src/DTVectorUtil.h",
        "src/DTQueueUtil.h",
        "src/JobQueue.h",
        "src/StdoutFileSink.h",
      ],
      "cflags!": [
        "-fno-exceptions",
        # "-frtti",
        # '-std=c++17',
        # '-fno-strict-aliasing',
        # '-fno-common',
        # '-fwrapv',
        # '-dynamic',
        # '-Wall',
      ],
      "cflags_cc!": [
        "-fno-exceptions",
        # "-frtti",
        # '-std=c++17',
        # '-fno-strict-aliasing',
        # '-fno-common',
        # '-fwrapv',
        # '-dynamic',
        # '-Wall',
      ],
      "link_settings": {
        "libraries": [
          "/usr/local/lib/libboost_date_time-mt.dylib",
          "/usr/local/lib/libboost_filesystem-mt.dylib",
          "/usr/local/lib/libboost_regex-mt.dylib",
          "/usr/local/lib/libboost_system-mt.dylib",
          "/usr/local/lib/libboost_thread-mt.dylib",
          "/usr/local/lib/libg3log.2.dylib",
          "/usr/local/lib/libmad.dylib",
          "/usr/local/lib/libmysqlclient.21.dylib",
          "/usr/local/lib/libmysqlcppconn.10.dylib",
          "/usr/local/lib/libtag.1.dylib",
        ],
      },
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '12.0',
          }
        }]
      ],
      "include_dirs": [
        "/Users/dogatech/.node-gyp/12.1.0/include/node",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
