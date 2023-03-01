{
  "targets": [
    {
      "target_name": "soulsifter",
      "cflags!": [ "-fno-exceptions" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      "sources": [
        # module
        "src/soulsifter.cc",
        # organization
        "src/MusicService.cpp",
        "src/MusicService_wrap.cpp",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}
