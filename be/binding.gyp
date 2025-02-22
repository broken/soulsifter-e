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
      "cflags": [
        "-fexceptions",
        "-frtti",
        # '-std=c++17',
        # '-fno-strict-aliasing',
        # '-fno-common',
        # '-fwrapv',
        # '-dynamic',
        # '-Wall',
      ],
      "cflags_cc": [
        "-fexceptions",
        "-frtti",
        # '-std=c++17',
        # '-fno-strict-aliasing',
        # '-fno-common',
        # '-fwrapv',
        # '-dynamic',
        # '-Wall',
      ],
      'conditions': [
        ['OS=="mac"', {
          'xcode_settings': {
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_ENABLE_CPP_RTTI': 'YES',
            'MACOSX_DEPLOYMENT_TARGET': '12.0',
          },
          'link_settings': {
            'libraries': [
              '/usr/local/lib/libboost_date_time-mt.dylib',
              '/usr/local/lib/libboost_filesystem-mt.dylib',
              '/usr/local/lib/libboost_regex-mt.dylib',
              '/usr/local/lib/libboost_system-mt.dylib',
              '/usr/local/lib/libboost_thread-mt.dylib',
              '/usr/local/lib/libg3log.2.dylib',
              '/usr/local/lib/libmad.dylib',
              '/usr/local/lib/libmysqlclient.24.dylib',
              '/usr/local/lib/libmysqlcppconn.10.dylib',
              '/usr/local/lib/libtag.1.dylib',
            ]
          }
        }],
        ['OS=="linux"', {
          'link_settings': {
            'libraries': [
              '/usr/lib/x86_64-linux-gnu/libboost_date_time.so',
              '/usr/lib/x86_64-linux-gnu/libboost_filesystem.so',
              '/usr/lib/x86_64-linux-gnu/libboost_regex.so',
              '/usr/lib/x86_64-linux-gnu/libboost_system.so',
              '/usr/lib/x86_64-linux-gnu/libboost_thread.so',
              '/usr/local/lib/libg3log.so.2',
              '/usr/lib/x86_64-linux-gnu/libmad.so.0',
              '/usr/lib/x86_64-linux-gnu/libmysqlclient.so.24',
              '/usr/lib/x86_64-linux-gnu/libmysqlcppconn.so.10',
              '/usr/lib/x86_64-linux-gnu/libtag.so.1',
            ]
          }
        }],
      ],
      "include_dirs": [
        "/Users/dogatech/.node-gyp/12.1.0/include/node",
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
