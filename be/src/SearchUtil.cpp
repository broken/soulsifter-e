//
//  Search.cpp
//  soul-sifter
//
//  Created by Robby Neale on 7/7/14.
//
//

#include "SearchUtil.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <boost/exception/all.hpp>
#include <boost/regex.hpp>
#include <boost/tokenizer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <g3log/g3log.hpp>

#include "Album.h"
#include "AlbumPart.h"
#include "DTVectorUtil.h"
#include "MusicVideo.h"
#include "MysqlAccess.h"
#include "Playlist.h"
#include "Song.h"
#include "SoulSifterSettings.h"
#include "Style.h"

using namespace std;

namespace dogatech {
namespace soulsifter {

namespace {

struct CamelotKeys {
  static boost::unordered_map<string, string> init() {
    boost::unordered_map<string, string> camelotKeys;
    camelotKeys["1A"] = "Abm";
    camelotKeys["1B"] = "B";
    camelotKeys["2A"] = "Ebm";
    camelotKeys["2B"] = "Gb";
    camelotKeys["3A"] = "Bbm";
    camelotKeys["3B"] = "Db";
    camelotKeys["4A"] = "Fm";
    camelotKeys["4B"] = "Ab";
    camelotKeys["5A"] = "Cm";
    camelotKeys["5B"] = "Eb";
    camelotKeys["6A"] = "Gm";
    camelotKeys["6B"] = "Bb";
    camelotKeys["7A"] = "Dm";
    camelotKeys["7B"] = "F";
    camelotKeys["8A"] = "Am";
    camelotKeys["8B"] = "C";
    camelotKeys["9A"] = "Em";
    camelotKeys["9B"] = "G";
    camelotKeys["10A"] = "Bm";
    camelotKeys["10B"] = "D";
    camelotKeys["11A"] = "Gbm";
    camelotKeys["11B"] = "A";
    camelotKeys["12A"] = "Dbm";
    camelotKeys["12B"] = "E";
    return camelotKeys;
  }
  static boost::unordered_map<string, int> initReverse() {
    boost::unordered_map<string, int> camelotKeys;
    camelotKeys["Abm"] = 17;
    camelotKeys["B"] = 5;
    camelotKeys["Ebm"] = 18;
    camelotKeys["Gb"] = 6;
    camelotKeys["Bbm"] = 19;
    camelotKeys["Db"] = 7;
    camelotKeys["Fm"] = 20;
    camelotKeys["Ab"] = 8;
    camelotKeys["Cm"] = 21;
    camelotKeys["Eb"] = 9;
    camelotKeys["Gm"] = 22;
    camelotKeys["Bb"] = 10;
    camelotKeys["Dm"] = 23;
    camelotKeys["F"] = 11;
    camelotKeys["Am"] = 12;
    camelotKeys["C"] = 0;
    camelotKeys["Em"] = 13;
    camelotKeys["G"] = 1;
    camelotKeys["Bm"] = 14;
    camelotKeys["D"] = 2;
    camelotKeys["Gbm"] = 15;
    camelotKeys["A"] = 3;
    camelotKeys["Dbm"] = 16;
    camelotKeys["E"] = 4;
    return camelotKeys;
  }
  static const boost::unordered_map<string, string> map;
  static const boost::unordered_map<string, int> rmap;
};

const boost::unordered_map<string, string> CamelotKeys::map = CamelotKeys::init();
const boost::unordered_map<string, int> CamelotKeys::rmap = CamelotKeys::initReverse();

struct Atom {
  enum Type {
    ANY,
    S_ID,
    S_ARTIST,
    S_TITLE,
    S_REMIXER,
    S_RATING,
    S_COMMENTS,
    S_CURATOR,
    S_ENERGY,
    S_BPM,
    S_TRASHED,
    S_LOW_QUALITY,
    A_ID,
    A_NAME,
    A_ARTIST,
    A_MIXED,
    A_LABEL,
    A_YEAR,
    A_MONTH,
    A_DAY,
    CUSTOM_QUERY_PREDICATE,
    LIMIT,
    ORDER_BY,
  };
  enum Property {
    NONE = 0x00,
    NEGATED = 0x01,
    CASE_SENSITIVE = 0x02,
    LESS_THAN = 0x04,
    GREATER_THAN = 0x08,
    EQUAL = 0x10,
  };
  string value;
  Type type;
  int props;

  void clear() {
    value = "";
    type = ANY;
    props = NONE;
  }
};

void splitString(const string& query, vector<string>* atoms) {
  boost::tokenizer<boost::escaped_list_separator<char> > tokenizer(query, boost::escaped_list_separator<char>('\\', ' ', '\"'));
  for (boost::tokenizer<boost::escaped_list_separator<char> >::iterator it = tokenizer.begin(); it != tokenizer.end(); ++it) {
    atoms->push_back(*it);
  }
}

bool parse(const string& queryFragment, Atom* atom) {
  atom->clear();
  boost::regex regex("^(-)?((id|a|artist|t|title|remixer|r|rating|comments|c|curator|e|energy|bpm|trashed|lowq|aid|n|album|m|mixed|l|label|y|year|month|day|q|query|limit|o|order|orderby|orderBy):)?(<|>)?(=)?(.+)$");
  boost::smatch match;
  if (!boost::regex_match(queryFragment, match, regex)) {
    return false;
  }
  if (match[1].length() > 0) {
    atom->props |= Atom::NEGATED;
  }
  if (match[4].length() > 0) {
    if (!match[4].compare("<")) atom->props |= Atom::LESS_THAN;
    else atom->props |= Atom::GREATER_THAN;
  }
  if (match[5].length() > 0) {
    atom->props |= Atom::EQUAL;
  }
  // set type
  if (match[2].length() > 0) {
    if (!match[3].compare("id")) {
      atom->type = Atom::S_ID;
    } else if (!match[3].compare("a") || !match[3].compare("artist")) {
      atom->type = Atom::S_ARTIST;
    } else if (!match[3].compare("t") || !match[3].compare("title")) {
      atom->type = Atom::S_TITLE;
    } else if (!match[3].compare("remixer")) {
      atom->type = Atom::S_REMIXER;
    } else if (!match[3].compare("r") || !match[3].compare("rating")) {
      atom->type = Atom::S_RATING;
    } else if (!match[3].compare("comments")) {
      atom->type = Atom::S_COMMENTS;
    } else if (!match[3].compare("c") || !match[3].compare("curator")) {
      atom->type = Atom::S_CURATOR;
    } else if (!match[3].compare("e") || !match[3].compare("energy")) {
      atom->type = Atom::S_ENERGY;
    } else if (!match[3].compare("bpm")) {
      atom->type = Atom::S_BPM;
    } else if (!match[3].compare("trashed")) {
      atom->type = Atom::S_TRASHED;
    } else if (!match[3].compare("lowq")) {
      atom->type = Atom::S_LOW_QUALITY;
    } else if (!match[3].compare("aid")) {
      atom->type = Atom::A_ID;
    } else if (!match[3].compare("n") || !match[3].compare("album")) {
      atom->type = Atom::A_NAME;
    } else if (!match[3].compare("m") || !match[3].compare("mixed")) {
      atom->type = Atom::A_MIXED;
    } else if (!match[3].compare("label")) {
      atom->type = Atom::A_LABEL;
    } else if (!match[3].compare("y") || !match[3].compare("year")) {
      atom->type = Atom::A_YEAR;
    } else if (!match[3].compare("month")) {
      atom->type = Atom::A_MONTH;
    } else if (!match[3].compare("day")) {
      atom->type = Atom::A_DAY;
    } else if (!match[3].compare("q") || !match[3].compare("query")) {
      atom->type = Atom::CUSTOM_QUERY_PREDICATE;
    } else if (!match[3].compare("limit")) {
      atom->type = Atom::LIMIT;
    } else if (!match[3].compare("l")) {
      // This is special, as l can be label or limit depending on usage
      boost::regex numRegex("^[0-9]+$");
      boost::smatch ignored;
      if (boost::regex_match(string(match[6]), ignored, numRegex)) {
        atom->type = Atom::LIMIT;
      } else {
        atom->type = Atom::A_LABEL;
      }
    } else if (!match[3].compare("o") || !match[3].compare("order") || !match[3].compare("orderby") || !match[3].compare("orderBy")) {
      atom->type = Atom::ORDER_BY;
    } else {
      // error
      return false;
    }
  }
  // set value
  if (atom->type == Atom::CUSTOM_QUERY_PREDICATE) {
    atom->value = string(match[6]);
  } else {
    boost::regex quoteRegex("'");
    atom->value = boost::regex_replace(string(match[6]), quoteRegex, "\\\\'");
  }
  return true;
}

string buildEqualityOperator(const int& props, const string& defaultOp = "=") {
  if (props & Atom::LESS_THAN && props & Atom::EQUAL) return "<=";
  if (props & Atom::LESS_THAN) return "<";
  if (props & Atom::GREATER_THAN && props & Atom::EQUAL) return ">=";
  if (props & Atom::GREATER_THAN) return ">";
  if (props & Atom::EQUAL) return "=";
  return defaultOp;
}

string buildQueryPredicate(const string& query, int* limit, int* energy, int* orderBy) {
  // Break query up into fragments
  vector<string> fragments;
  splitString(query, &fragments);

  stringstream ss;
  for (string fragment : fragments) {
    Atom atom;
    if (!parse(fragment, &atom)) {
      LOG(WARNING) << "ERROR: Unable to parse query fragment '" << fragment << "'";
      continue;
    }

    ss << " and ";
    if (atom.props & Atom::NEGATED) {
      ss << "not ";
    }
    if (atom.type == Atom::ANY) {
      ss << "(ifnull(s.artist,'') like '%" << atom.value << "%' or ifnull(s.title,'') like '%" << atom.value << "%' or ifnull(s.remixer,'') like '%" << atom.value << "%' or ifnull(s.comments,'') like '%" << atom.value << "%' or ifnull(s.curator,'') like '%" << atom.value << "%' or ifnull(a.name,'') like '%" << atom.value << "%')";
    } else if (atom.type == Atom::S_ID) {
      ss << "s.id " << buildEqualityOperator(atom.props) << " " << atom.value;
    } else if (atom.type == Atom::S_ARTIST) {
      ss << "ifnull(s.artist,'') ";
      if (atom.props & (Atom::LESS_THAN | Atom::GREATER_THAN | Atom::EQUAL))
        ss << buildEqualityOperator(atom.props) << " '" << atom.value << "'";
      else
        ss << "like '%" << atom.value << "%'";
    } else if (atom.type == Atom::S_TITLE) {
      ss << "ifnull(s.title,'') ";
      if (atom.props & (Atom::LESS_THAN | Atom::GREATER_THAN | Atom::EQUAL))
        ss << buildEqualityOperator(atom.props) << " '" << atom.value << "'";
      else
        ss << "like '%" << atom.value << "%'";
    } else if (atom.type == Atom::S_REMIXER) {
      ss << "ifnull(s.remixer,'') like '%" << atom.value << "%'";
    } else if (atom.type == Atom::S_RATING) {
      ss << "s.rating " << buildEqualityOperator(atom.props, ">=") << " " << atom.value;
    } else if (atom.type == Atom::S_COMMENTS) {
      ss << "ifnull(s.comments,'') like '%" << atom.value << "%'";
    } else if (atom.type == Atom::S_CURATOR) {
      ss << "ifnull(s.curator,'') like '%" << atom.value << "%'";
    } else if (atom.type == Atom::S_TRASHED) {
      ss << "s.trashed = " << atom.value;
    } else if (atom.type == Atom::S_LOW_QUALITY) {
      ss << "s.lowQuality = " << atom.value;
    } else if (atom.type == Atom::A_ID) {
      ss << "a.id " << buildEqualityOperator(atom.props) << " " << atom.value;
    } else if (atom.type == Atom::A_NAME) {
      ss << "ifnull(a.name,'') like '%" << atom.value << "%'";
    } else if (atom.type == Atom::A_MIXED) {
      ss << "a.mixed = " << atom.value;
    } else if (atom.type == Atom::A_LABEL) {
      ss << "ifnull(a.label,'') like '%" << atom.value << "%'";
    } else if (atom.type == Atom::A_YEAR) {
      ss << "a.releaseDateYear " << buildEqualityOperator(atom.props) << " " << atom.value;
    } else if (atom.type == Atom::A_MONTH) {
      ss << "a.releaseDateMonth " << buildEqualityOperator(atom.props) << " " << atom.value;
    } else if (atom.type == Atom::A_DAY) {
      ss << "a.releaseDateDay " << buildEqualityOperator(atom.props) << " " << atom.value;
    } else if (atom.type == Atom::CUSTOM_QUERY_PREDICATE) {
      ss << atom.value;
    } else if (atom.type == Atom::S_BPM) {
      // can be a single number or a range
      size_t pos;
      int min_bpm = 0, max_bpm = 0;
      if ((pos = atom.value.find("-")) != string::npos) {
        min_bpm = atoi(atom.value.substr(0, pos).c_str());
        max_bpm = atoi(atom.value.substr(pos + 1, atom.value.length()).c_str());
      } else if (!buildEqualityOperator(atom.props, "").empty()) {
        ss << "s.bpm " << buildEqualityOperator(atom.props) << " " << atom.value;
      } else {
        min_bpm = atoi(atom.value.c_str());
        max_bpm = min_bpm + 1;
      }
      if (min_bpm > 0 && max_bpm > 0) {
        ss << "(s.bpm between " << min_bpm << " and " << max_bpm;
        if (max_bpm > 120) ss << " or s.bpm between " << min_bpm / 2 << " and " << max_bpm / 2;
        if (min_bpm <= 100) ss << " or s.bpm between " << min_bpm * 2 << " and " << max_bpm * 2;
        ss << ")";
      }
    } else if (atom.type == Atom::LIMIT) {
      *limit = atoi(atom.value.c_str());
      ss << "true";
    } else if (atom.type == Atom::S_ENERGY) {
      // If an operator property is specified, it will overwrite what is used in the options build. Otherwise, we default to it.
      if (atom.props == 0) {
        *energy = atoi(atom.value.c_str());
        ss << "true";
      } else {
        ss << "s.energy " << buildEqualityOperator(atom.props) << " " << atom.value;
      }
    } else if (atom.type == Atom::ORDER_BY) {
      if (!atom.value.compare("rand") || !atom.value.compare("random")) {
        *orderBy = RANDOM;
      } else if (!atom.value.compare("release_date") || !atom.value.compare("rdate") || !atom.value.compare("date_released") || !atom.value.compare("released")) {
        *orderBy = RELEASE_DATE;
      } else if (!atom.value.compare("added_date") || !atom.value.compare("adate") || !atom.value.compare("date_added") || !atom.value.compare("added")) {
        *orderBy = DATE_ADDED;
      } else if (!atom.value.compare("bpm")) {
        *orderBy = BPM;
      } else if (!atom.value.compare("album")) {
        *orderBy = ALBUM;
      } else if (!atom.value.compare("playlist")) {
        *orderBy = PLAYLIST;
      }
      ss << "true";
    }
  }
  return ss.str();
}

string buildOptionPredicate(const int bpm, const string& key, const vector<Style*>& styles, const vector<Song*>& songsToOmit, const vector<Playlist*>& playlists, const int limit, const int energy, const int orderBy) {
  stringstream ss;
  if (CamelotKeys::rmap.find(key) != CamelotKeys::rmap.end()) {
    // assume key lock always on for now
    ss << " and (";
    int num = 0;
    switch (CamelotKeys::rmap.at(key)) {  // 0 - 11 B , 12 - 23 A
      case 5:  // B = 1B
        ss << "tonicKey = '" << CamelotKeys::map.at("1A") << "' or tonicKey = '" << CamelotKeys::map.at("12B") << "'";
        if (++num == 3) break;
      case 6:  // Gb = 2B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("2A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("1B") << "'";
        if (++num == 3) break;
      case 7:  // Db = 3B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("3A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("2B") << "'";
        if (++num == 3) break;
      case 8:  // Ab = 4B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("4A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("3B") << "'";
        if (++num == 3) break;
      case 9:  // Eb = 5B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("5A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("4B") << "'";
        if (++num == 3) break;
      case 10: // Bb = 6B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("6A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("5B") << "'";
        if (++num == 3) break;
      case 11: // F = 7B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("7A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("6B") << "'";
        if (++num == 3) break;
      case 0:  // C = 8B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("8A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("7B") << "'";
        if (++num == 3) break;
      case 1:  // G = 9B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("9A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("8B") << "'";
        if (++num == 3) break;
      case 2:  // D = 10B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("10A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("9B") << "'";
        if (++num == 3) break;
      case 3:  // A = 11B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("11A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("10B") << "'";
        if (++num == 3) break;
      case 4:  // E = 12B
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("12A") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("11B") << "'";
        if (++num == 3) break;
        ss << " or tonicKey = '" << CamelotKeys::map.at("12B") << "'";
        if (++num == 3) break;
        ss << " or tonicKey = '" << CamelotKeys::map.at("1B") << "'";
        if (++num == 3) break;
    // 0 - 11 B (above), 12 - 23 A (below)
      case 17: // Abm = 1A
        ss << "tonicKey = '" << CamelotKeys::map.at("1B") << "' or tonicKey = '" << CamelotKeys::map.at("12A") << "'";
        if (++num == 3) break;
      case 18: // Ebm = 2A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("2B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("1A") << "'";
        if (++num == 3) break;
      case 19: // Bbm = 3A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("3B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("2A") << "'";
        if (++num == 3) break;
      case 20: // Fm = 4A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("4B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("3A") << "'";
        if (++num == 3) break;
      case 21: // Cm = 5A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("5B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("4A") << "'";
        if (++num == 3) break;
      case 22: // Gm = 6A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("6B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("5A") << "'";
        if (++num == 3) break;
      case 23: // Dm = 7A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("7B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("6A") << "'";
        if (++num == 3) break;
      case 12: // Am = 8A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("8B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("7A") << "'";
        if (++num == 3) break;
      case 13: // Em = 9A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("9B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("8A") << "'";
        if (++num == 3) break;
      case 14: // Bm = 10A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("10B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("9A") << "'";
        if (++num == 3) break;
      case 15: // Gbm = 11A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("11B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("10A") << "'";
        if (++num == 3) break;
      case 16: // Csm = 12A
        if (num == 0) ss << "tonicKey = '" << CamelotKeys::map.at("12B") << "'";
        ss << " or tonicKey = '" << CamelotKeys::map.at("11A") << "'";
        if (++num == 3) break;
        ss << " or tonicKey = '" << CamelotKeys::map.at("12A") << "'";
        if (++num == 3) break;
        ss << " or tonicKey = '" << CamelotKeys::map.at("1A") << "'";
        if (++num == 3) break;
      default:
        // TODO bubble up this error
        LOG(WARNING) << "Error. Unable to find key.";
        return "";
    }
    if (SoulSifterSettings::getInstance().get<bool>("search.includeUnknownKeys")) ss << " or tonicKey is null";
    ss << ")";
  }
  int pitchPctMax = 8;  // TODO should be a setting
  int max_bpm = bpm * (100 + pitchPctMax) / 100;
  int min_bpm = bpm * (100 - pitchPctMax) / 100;
  if (max_bpm > 0 && min_bpm > 0) {
    ss << " and (bpm between " << min_bpm << " and " << max_bpm;
    if (max_bpm > 120) ss << " or bpm between " << min_bpm / 2 << " and " << max_bpm / 2;
    if (min_bpm <= 90) ss << " or bpm between " << min_bpm * 2 << " and " << max_bpm * 2;
    ss << ")";
  }
  if (energy > 0) {
    int diff = SoulSifterSettings::getInstance().get<int>("search.energyGap");
    ss << " and (s.energy between " << energy - diff << " and " << energy + diff << ")";
  }
  if (styles.size() > 0) {
    ss << " and exists (select 1 from SongStyles g where s.id = g.songId and g.styleId in (";
    string separator("");
    for (Style* s : styles) {
      ss << separator << s->getId();
      separator = ",";
    }
    ss << "))";
  }
  if (songsToOmit.size() > 0) {
    ss << " and s.id not in (";
    string separator("");
    for (Song* s : songsToOmit) {
      ss << separator << s->getId();
      separator = ",";
    }
    ss << ")";
  }
  if (playlists.size() > 0) {
    ss << " and pe.playlistid in (";
    string separator("");
    for (Playlist* p : playlists) {
      ss << separator << p->getId();
      separator = ",";
    }
    ss << ")";
  }

  ss << " group by " << (playlists.size() > 0 ? "pe.id, " : "") << "s.id order by ";
  if (orderBy == RELEASE_DATE) {
    ss << "a.releaseDateYear desc, a.releaseDateMonth desc, a.releaseDateDay desc";
  } else if (orderBy == RANDOM) {
    ss << "rand()";
  } else if (orderBy == BPM) {
    ss << "s.bpm asc";
  } else if (orderBy == PLAYLIST) {
    ss << "pe.playlistid desc, pe.position asc";
  } else if (orderBy == ALBUM) {
    ss << "a.id desc, cast(s.track as unsigned) asc, s.track asc";
  } else {
    ss << "s.id desc";
  }
  ss << " limit " << limit;
  return ss.str();
}

}  // anon namespace

vector<Song*>* SearchUtil::searchSongs(const string& query,
                                       int limit,
                                       const int bpm,
                                       const string& key,
                                       const vector<Style*>& styles,
                                       const vector<Song*>& songsToOmit,
                                       const vector<Playlist*>& playlists,
                                       int energy,
                                       const bool musicVideoMode,
                                       int orderBy,
                                       std::function<void(string)> errorCallback) {
  LOG(INFO) << "q:" << query << ", bpm:" << bpm << ", key:" << key << ", styles:" << ", limit:" << limit;
  vector<Song*>* songs = new vector<Song*>();

  stringstream ss;
  if (musicVideoMode)
    ss << "select s.*, s.id as songid, s.artist as songartist, group_concat(ss.styleid) as styleIds, a.*, a.id as albumid, a.artist as albumartist, v.filepath as mvFilePath, v.thumbnailFilePath as mvTnFilePath from Songs s inner join Albums a on s.albumid = a.id inner join MusicVideos v on s.musicVideoId=v.id left outer join SongStyles ss on ss.songid=s.id where true";
  else if (playlists.size() > 0)
    ss << "select s.*, s.id as songid, s.artist as songartist, group_concat(ss.styleid) as styleIds, a.*, a.id as albumid, a.artist as albumartist from PlaylistEntries pe left outer join Songs s on pe.songid=s.id inner join Albums a on s.albumid = a.id left outer join SongStyles ss on ss.songid=s.id where true";
  else
    ss << "select s.*, s.id as songid, s.artist as songartist, group_concat(ss.styleid) as styleIds, a.*, a.id as albumid, a.artist as albumartist from Songs s inner join Albums a on s.albumid = a.id left outer join SongStyles ss on ss.songid=s.id where true";
  try {
    ss << buildQueryPredicate(query, &limit, &energy, &orderBy);
    ss << buildOptionPredicate(bpm, key, styles, songsToOmit, playlists, limit, energy, orderBy);
  } catch (boost::exception &e) {
    LOG(WARNING) << "ERROR: Error parsing query. " << boost::diagnostic_information(e);
    if (errorCallback) errorCallback(boost::diagnostic_information(e));
    else LOG(WARNING) << "Undefined callback. Unable to send error.";
    return songs;
  }

  LOG(DEBUG) << "Query:";
  LOG(DEBUG) << ss.str();

  int i = 0;
  for (; i < 2; ++i) {
    try {
      sql::Statement *stmt = MysqlAccess::getInstance().createStatement();
      sql::ResultSet *rs = stmt->executeQuery(ss.str());
      while (rs->next()) {
        Song* song = new Song();
        song->populateFields(rs, song);
        song->setId(rs->getInt("songid"));
        song->setArtist(rs->getString("songartist"));

        Album* album = new Album();
        album->populateFields(rs, album);
        album->setId(rs->getInt("albumid"));
        album->setArtist(rs->getString("albumartist"));
        song->setAlbum(album);

        if (musicVideoMode) {
          MusicVideo* video = new MusicVideo();
          video->setId(rs->getInt("musicVideoId"));
          video->setFilePath(rs->getString("mvFilePath"));
          video->setThumbnailFilePath(rs->getString("mvTnFilePath"));
          song->setMusicVideo(video);
        }

        songs->push_back(song);
      }
      rs->close();
      delete rs;
      delete stmt;

      return songs;
    } catch (sql::SQLException &e) {
      LOG(WARNING) << "ERROR: SQLException in " << __FILE__ << " (" << __func__<< ") on line " << __LINE__;
      LOG(WARNING) << "ERROR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")";
      if (i == 0) {
        bool reconnected = MysqlAccess::getInstance().reconnect();
        LOG(INFO) << (reconnected ? "Successful" : "Failed") << " mysql reconnection";
      } else {
        LOG(WARNING) << e.what();
        if (errorCallback) errorCallback(e.what());
        else LOG(WARNING) << "Undefined callback. Unable to send error.";
      }
    }
  }

  return songs;
}

}  // namespace soulsifter
}  // namespace dogatech
