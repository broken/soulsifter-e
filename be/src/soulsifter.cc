#include <napi.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <napi.h>
#include <g3log/g3log.hpp>
#include <g3log/logworker.hpp>
#include "Album_wrap.h"
#include "AlbumPart_wrap.h"
#include "AudioAnalyzer_wrap.h"
#include "BasicGenre_wrap.h"
#include "Mix_wrap.h"
#include "MusicService_wrap.h"
#include "MusicVideo_wrap.h"
#include "MusicVideoService_wrap.h"
#include "NewSongManager_wrap.h"
#include "Playlist_wrap.h"
#include "PlaylistEntry_wrap.h"
#include "SearchUtil_wrap.h"
#include "Song_wrap.h"
#include "SoulSifterSettings_wrap.h"
#include "StdoutFileSink.h"
#include "Style_wrap.h"
#include "TagService_wrap.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  // first init the logging
  static std::unique_ptr<g3::LogWorker> logworker = g3::LogWorker::createLogWorker();
  const std::string home = getenv("HOME");
  const std::string logDir = home + "/Library/Logs/SoulSifter";
  if (!std::filesystem::exists(logDir)) {
    if (!std::filesystem::create_directories(logDir)) {
      std::cerr << "Failed to create log directory: " << logDir << std::endl;
    }
  }
  logworker->addSink(
      std::make_unique<dogatech::soulsifter::StdoutFileSink>("ss", logDir),
      &dogatech::soulsifter::StdoutFileSink::log);
  g3::initializeLogging(logworker.get());

  // model
  Album::Init(env, exports);
  AlbumPart::Init(env, exports);
  BasicGenre::Init(env, exports);
  Mix::Init(env, exports);
  MusicVideo::Init(env, exports);
  Playlist::Init(env, exports);
  PlaylistEntry::Init(env, exports);
  Song::Init(env, exports);
  Style::Init(env, exports);

  // services
  AudioAnalyzer::Init(env, exports);
  MusicService::Init(env, exports);
  MusicVideoService::Init(env, exports);
  NewSongManager::Init(env, exports);
  SearchUtil::Init(env, exports);
  TagService::Init(env, exports);

  // not auto-generated
  SoulSifterSettings::Init(env, exports);

  return  exports;
}

NODE_API_MODULE(soulsifter, InitAll)