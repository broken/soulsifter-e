//
//  AudioAnalyzer.cpp
//  soul-sifter
//
//  Created by Robby Neale on 11/9/13.
//
//

#include "AudioAnalyzer.h"

#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/stat.h>
#include <utility>
#include <vector>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <g3log/g3log.hpp>
#include <taglib/mpegfile.h>

#include "DTVectorUtil.h"
#include "madlld.h"
#include "MiniBpm.h"
#include "SearchUtil.h"
#include "Song.h"
#include "SoulSifterSettings.h"

namespace dogatech {
  namespace soulsifter {
    
    namespace {
      bool keyPairCompare(std::pair<string, float> first, std::pair<string, float> second) {
        return first.second > second.second;
      }
    }
    
    const vector<double> AudioAnalyzer::analyzeBpm(Song *song) {
      LOG(INFO) << "analyze bpm";

      struct stat statBuffer;
      string songFilepath;
      if (stat(song->getFilepath().c_str(), &statBuffer) == 0) {
        songFilepath = song->getFilepath();
      } else if (stat((SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath()).c_str(), &statBuffer) == 0) {
        songFilepath = SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath();
      } else {
        LOG(WARNING) << "File does not exist for song " << song->getId();
        vector<double> bpms;
        return bpms;
      }
      
      TagLib::MPEG::File f(songFilepath.c_str());
      int sampleRate = f.audioProperties()->sampleRate();
      if (sampleRate != 44100) {
        LOG(INFO) << "Sample rate discovered to be " << sampleRate << " instead of 44100.";
      }
      breakfastquay::MiniBPM miniBpm(sampleRate);
      
      if (boost::algorithm::iends_with(songFilepath, ".mp3")) {
        detectBpm(songFilepath.c_str(), boost::bind(&breakfastquay::MiniBPM::process, boost::ref(miniBpm), _1, _2));
      } else {
        LOG(WARNING) << "Unrecognizable file type when analyzing BPM for " << songFilepath;
        vector<double> bpms;
        return bpms;
      }
    
      char buffer[8];
      sprintf(buffer, "%.2f", miniBpm.estimateTempo());
      song->setBpm(buffer);
      
      return miniBpm.getTempoCandidates();
    }

    void AudioAnalyzer::analyzeBpms() {
      LOG(INFO) << "analyze bpms";

      vector<Style*> emptyStyles;
      vector<Song*> emptySongs;
      string query = "id:\"(select max(id) from songs)\"";
      vector<Song*>* songs = SearchUtil::searchSongs(query, 0, "", emptyStyles, emptySongs, 1);
      int maxId = 0;
      for (Song* song : *songs) {
        maxId = song->getId();
      }
      deleteVectorPointers(songs);

      int span = 100;
      for (int i = 0; i <= maxId; i += span) {
        stringstream ss;
        ss << "q:\"s.id >= " << i << "\" q:\"s.id < " << (i + span) << "\"";
        ss << " trashed:0 q:\"bpm is null\"";
        query = ss.str();
        songs = SearchUtil::searchSongs(query, 0, "", emptyStyles, emptySongs, span);

        for (Song* song : *songs) {
          string bpm = song->getBpm();
          AudioAnalyzer::analyzeBpm(song);
          if (bpm.compare(song->getBpm())) {
            song->update();
          }
        }
        deleteVectorPointers(songs);
      }
    }
    
    int AudioAnalyzer::analyzeDuration(Song* song) {
      LOG(INFO) << "analyze duration";

      struct stat statBuffer;
      string songFilepath;
      if (stat(song->getFilepath().c_str(), &statBuffer) == 0) {
        songFilepath = song->getFilepath();
      } else if (stat((SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath()).c_str(), &statBuffer) == 0) {
        songFilepath = SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath();
      } else {
        LOG(WARNING) << "File does not exist for song " << song->getId();
        return 0;
      }

      TagLib::MPEG::File f(songFilepath.c_str());
      song->setDurationInMs(f.audioProperties()->lengthInMilliseconds());
      return song->getDurationInMs();
    }

    void AudioAnalyzer::analyzeDurations() {
      LOG(INFO) << "analyze durations";

      vector<Style*> emptyStyles;
      vector<Song*> emptySongs;
      string query = "id:\"(select max(id) from songs)\"";
      vector<Song*>* songs = SearchUtil::searchSongs(query, 0, "", emptyStyles, emptySongs, 1);
      int maxId = 0;
      for (Song* song : *songs) {
        maxId = song->getId();
      }
      deleteVectorPointers(songs);

      int span = 100;
      for (int i = 0; i <= maxId; i += span) {
        stringstream ss;
        ss << "q:\"s.id >= " << i << "\" q:\"s.id < " << (i + span) << "\"";
        ss << " trashed:0";  // todo time is null
        query = ss.str();
        songs = SearchUtil::searchSongs(query, 0, "", emptyStyles, emptySongs, span);
        for (Song* song : *songs) {
          int duration = song->getDurationInMs();
          AudioAnalyzer::analyzeDuration(song);
          if (duration != song->getDurationInMs()) {
            song->update();
          }
        }
        deleteVectorPointers(songs);
      }
    }

    const Keys* AudioAnalyzer::analyzeKey(Song *song) {
      LOG(INFO) << "analyze key";
      
      FILE *fpipe;
      stringstream command;
      command << "/Users/rneale/sonic-annotator -d vamp:qm-vamp-plugins:qm-keydetector:key -w csv --csv-stdout ";
      command << "\"" << SoulSifterSettings::getInstance().get<string>("music.dir") << song->getFilepath() << "\"";
      char buffer[1024];
      
      if (!(fpipe = (FILE*)popen(command.str().c_str(), "r")) ) {
        // If fpipe is NULL
        LOG(WARNING) << "Problems with sonic annotator pipe.";
        return new Keys();
      }
      
      stringstream ss;
      while (fgets(buffer, sizeof buffer, fpipe)) {
        ss << buffer;
      }
      
      pclose(fpipe);
      
      float lastTime(0);
      map<string, float> keys;
      string output(ss.str());
      LOG(INFO) << output;
      vector<string> lines;
      boost::split(lines, output, boost::is_any_of("\n"));
      for (string line : lines) {
        vector<string> parts;
        boost::split(parts, line, boost::is_any_of(","));
        if (parts.size() == 4) {
          float time = stof(parts[1]);
          if (time > 10) {
            string strKey = parts[3].substr(1, parts[3].length()-2);
            if (keys.count(strKey) > 0) {
              keys[strKey] += time - lastTime;
            } else {
              keys[strKey] = time - lastTime;
            }
          }
          lastTime = time;
        }
      }
      
      Keys* results = new Keys(keys.begin(), keys.end());
      sort(results->candidate.begin(), results->candidate.end(), &keyPairCompare);

      return results;
    }
    
  }
}
