#include "TagService.h"

#include <cmath>
#include <functional>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include <g3log/g3log.hpp>
#include <taglib/attachedpictureframe.h>
#include <taglib/fileref.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2tag.h>
#include <taglib/mp4file.h>
#include <taglib/mpegfile.h>
#include <taglib/popularimeterframe.h>
#include <taglib/tbytevector.h>
#include <taglib/textidentificationframe.h>
#include <taglib/tpropertymap.h>
#include <taglib/tstring.h>
#include <taglib/tstringlist.h>

#include "Album.h"
#include "AlbumPart.h"
#include "DTVectorUtil.h"
#include "SearchUtil.h"
#include "Song.h"
#include "SoulSifterSettings.h"
#include "Style.h"

namespace dogatech {
namespace soulsifter {

namespace {

// custom tag descriptions
const char* const CURATOR = "CURATOR";
const char* const LOW_QUALITY = "LQ";
const char* const YOUTUBE = "YOUTUBE";

class ImageFile : public TagLib::File {
public:
  ImageFile(const char *file) : TagLib::File(file) { }

  TagLib::String mimeType() const {
    string filename = name();

    if (boost::algorithm::iends_with(filename, "jpg") ||
        boost::algorithm::iends_with(filename, "jpeg")) {
      return "image/jpeg";
    } else if (boost::algorithm::iends_with(filename, "gif")) {
      return "image/gif";
    } else if (boost::algorithm::iends_with(filename, "png")) {
      return "image/png";
    } else {
      LOG(WARNING) << "Unknown image type for file " << filename;
      return TagLib::String::null;
    }
  }

  TagLib::ByteVector data() {
    return readBlock(length());
  }

  bool isValid() const {
    return isOpen() && !mimeType().isNull();
  }

private:
  virtual TagLib::Tag *tag() const { return NULL; }
  virtual TagLib::AudioProperties *audioProperties() const { return NULL; }
  virtual bool save() { return false; }
};

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

float strToFloat(const string& s) {
  stringstream ss(s);
  float x;
  ss >> x;
  return x;
}

int canonicalizeBpm(const int bpm) {
  if (bpm <= 0) return 0;
  if (bpm < 75) return canonicalizeBpm(bpm << 1);
  if (bpm >= 150) return canonicalizeBpm(bpm >> 1);
  return bpm;
}

int canonicalizeBpm(const string& bpm) {
  return canonicalizeBpm((int)(strToFloat(bpm) + 0.5));
}
    
const string getId3v2Text(TagLib::ID3v2::Tag* id3v2, const char* name) {
  TagLib::ID3v2::FrameList frameList = id3v2->frameListMap()[name];
  return frameList.isEmpty() ? "" : trim_copy(frameList.front()->toString().to8Bit());
}

const string getId3v2UserText(TagLib::ID3v2::Tag* id3v2, const char* description) {
  TagLib::ID3v2::UserTextIdentificationFrame* frame = TagLib::ID3v2::UserTextIdentificationFrame::find(id3v2, description);
  return !frame ? "" : trim_copy(frame->fieldList()[1].to8Bit());  // the desciption is copied into the first element of the field list
}

bool readId3v2TagAttributes(Song* song, TagLib::ID3v2::Tag* id3v2) {
  LOG(INFO) << "reading tags for song " << song->getId();
  bool updated = false;
  bool overwrite = SoulSifterSettings::getInstance().get<bool>("tag.readOverwrite");  // TODO: make option instead of setting
  if (!id3v2 || !song) {
    return updated;
  }
  // bpm
  const string bpm = getId3v2Text(id3v2, "TBPM");
  if (!song->getBpmLock()
      && canonicalizeBpm(bpm) != canonicalizeBpm(song->getBpm())
      && (overwrite || !canonicalizeBpm(song->getBpm()))) {
    LOG(DEBUG) << "updating song " << song->getId()
         << " bpm from " << song->getBpm()
         << " to " << bpm;
    if (abs(canonicalizeBpm(bpm) - canonicalizeBpm(song->getBpm())) > 3) {
      LOG(WARNING) << "BPM for song " << song->getId() << " was WAY off!";
    }
    if (canonicalizeBpm(bpm) > 0) {
      song->setBpm(bpm);
      updated = true;
    }
  }
  // energy
  const string grpTag = getId3v2Text(id3v2, "TIT1");
  boost::regex grpRegex("^(\\d+(?![\\w\\d]))?(?: - )?(?:1?\\d[AB] (.{1,3}))?$");
  boost::smatch grpMatch;
  string oldKey;
  if (boost::regex_search(grpTag, grpMatch, grpRegex, boost::match_default)) {
    string energystr = grpMatch[1];
    oldKey = grpMatch[2];
    LOG(DEBUG) << "song " << song->getId() << " from " << grpTag
               << " found energy " << energystr
               << " and key " << oldKey;
    int energy = std::atoi(energystr.c_str());
    if (energy != song->getEnergy() && (overwrite || !song->getEnergy())) {
      LOG(INFO) << "updating song " << song->getId()
                << " energy from " << song->getEnergy()
                << " to " << energy;
      if (energy > 0) {
        song->setEnergy(energy);
        updated = true;
      }
    }
  }
  // key
  string initialKey = getId3v2Text(id3v2, "TKEY");
  if (!song->getTonicKeyLock() && !initialKey.empty()) {
    size_t pos;
    if ((pos = initialKey.find("\\")) != string::npos) {
      initialKey = initialKey.substr(0, pos);
    }
    if (initialKey != song->getTonicKey() &&
        (overwrite || song->getTonicKey().empty()) &&
        initialKey != "All") {
      LOG(INFO) << "updating song " << song->getId()
           << " key from " << song->getTonicKey()
           << " to " << initialKey;
      // convert key from sharp to flat
      if (initialKey.length() >= 2 && initialKey[1] == '#') {
        if (initialKey[0] == 'G') initialKey[0] = 'A';
        else initialKey[0] = initialKey[0] + 1;
        initialKey[1] = 'b';
      }
      song->setTonicKey(initialKey);
      updated = true;
    }
  }

  return updated;
}

bool readId3v2TagAttributes(Song* song) {
  TagLib::MPEG::File f((SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath()).c_str());
  TagLib::ID3v2::Tag* id3v2 = f.ID3v2Tag();  // still owned by file
  return readId3v2TagAttributes(song, id3v2);
}

void setId3v2Text(TagLib::ID3v2::Tag* id3v2, const char* name, const char* val) {
  TagLib::ByteVector handle = name;
  TagLib::String value = val;

  if(!id3v2->frameList(handle).isEmpty()) {
    id3v2->frameList(handle).front()->setText(value);
  } else {
    TagLib::ID3v2::TextIdentificationFrame *frame = new TagLib::ID3v2::TextIdentificationFrame(handle, TagLib::String::UTF8);
    id3v2->addFrame(frame);
    frame->setText(value);
  }
}

void setId3v2UserText(TagLib::ID3v2::Tag* id3v2, const char* description, const char* val) {
  TagLib::StringList values(val);

  TagLib::ID3v2::UserTextIdentificationFrame* frame = TagLib::ID3v2::UserTextIdentificationFrame::find(id3v2, description);
  if(frame) {
    frame->setText(values);
  } else {
    frame = new TagLib::ID3v2::UserTextIdentificationFrame(description, values, TagLib::String::UTF8);
    id3v2->addFrame(frame);
  }
}

// other formats: https://gist.github.com/guymac/1468279
void setId3v2Picture(TagLib::ID3v2::Tag* id3v2, string path, bool replace) {
  TagLib::ID3v2::FrameList frames = id3v2->frameListMap()["APIC"];  // get pictures
  if (!frames.isEmpty() && !replace) {
    LOG(INFO) << "Picture exists: not replacing.";
    return;
  } else if (!frames.isEmpty() && replace) {
    id3v2->removeFrames("APIC");
  }
  ImageFile image(path.c_str());
  if (!image.isValid()) {
    return;
  }
  TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;
  frame->setMimeType(image.mimeType());
  frame->setPicture(image.data());
  id3v2->addFrame(frame);
}

}  // namespace
  
void TagService::readId3v2Tag(Song* song) {
  string songFilepath = song->getFilepath();
  if (!boost::filesystem::exists(songFilepath)) {
    songFilepath = SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath();
    if (!boost::filesystem::exists(songFilepath)) {
      LOG(WARNING) << "unable to find song: " << song->getFilepath();
    }
  }
  TagLib::MPEG::File f(songFilepath.c_str());
  TagLib::ID3v2::Tag* id3v2 = f.ID3v2Tag();
  if (!id3v2) {
    return;
  }

    if (id3v2->artist() != TagLib::String::null) song->setArtist(trim_copy(id3v2->artist().to8Bit()));
    song->setTrack(getId3v2Text(id3v2, "TRCK"));
    if (id3v2->title() != TagLib::String::null) song->setTitle(trim_copy(id3v2->title().to8Bit()));
    song->setRemixer(getId3v2Text(id3v2, "TPE4"));
    song->getAlbum()->setArtist(getId3v2Text(id3v2, "TPE2"));
    if (id3v2->album() != TagLib::String::null) song->getAlbum()->setName(trim_copy(id3v2->album().to8Bit()));
    song->getAlbum()->setLabel(getId3v2Text(id3v2, "TPUB"));
    song->getAlbum()->setCatalogId(getId3v2Text(id3v2, "TCID"));
    if (id3v2->year() != 0) song->getAlbum()->setReleaseDateYear(id3v2->year());
    //TODO if (id3v2->genre() != TagLib::String::null) song->setGenre(id3v2->genre().to8Bit());
    if (id3v2->comment() != TagLib::String::null) song->setComments(trim_copy(id3v2->comment().to8Bit()));
    
    TagLib::ID3v2::FrameList frameList = id3v2->frameListMap()["POPM"];
    if (!frameList.isEmpty()) {
      TagLib::ID3v2::PopularimeterFrame *popm = static_cast<TagLib::ID3v2::PopularimeterFrame*>(frameList.front());
      song->setRating(popm->rating());
    }
    
    // part of set
    const string pos = getId3v2Text(id3v2, "TPOS");
    if (pos.length() > 0) {
      if (!song->getAlbumPart()) {
        AlbumPart ap;
        song->setAlbumPart(ap);
      }
      song->getAlbumPart()->setAlbum(*song->getAlbum());
      song->getAlbumPart()->setPos(pos);
    }
    
    // string in the DDMM format
    const string date = getId3v2Text(id3v2, "TDAT");
    char *tmp = new char[3];
    tmp[0] = date[0];
    tmp[1] = date[1];
    tmp[2] = '\0';
    if (tmp[0] != '0' || tmp[1] != '0')
      song->getAlbum()->setReleaseDateDay(atoi(tmp));
    tmp[0] = date[2];
    tmp[1] = date[3];
    song->getAlbum()->setReleaseDateMonth(atoi(tmp));
    delete [] tmp;

  // custom tags
  string lq(getId3v2UserText(id3v2, LOW_QUALITY));
  song->setLowQuality(lq.length() > 0 && lq.compare("0") && lq.compare("false"));
  song->setCurator(getId3v2UserText(id3v2, CURATOR));
  song->setYoutubeId(getId3v2UserText(id3v2, YOUTUBE));

  // attributes (bpm, key, energy)
  readId3v2TagAttributes(song, id3v2);
}

void TagService::writeId3v2Tag(Song* song) {
  if (boost::algorithm::iends_with(song->getFilepath(), ".mp3") ||
      boost::algorithm::iends_with(song->getFilepath(), ".m4a") ||
      boost::algorithm::iends_with(song->getFilepath(), ".mp4") ||
      boost::algorithm::iends_with(song->getFilepath(), ".aac") ||
      boost::algorithm::iends_with(song->getFilepath(), ".alac")) {
    string songFilepath = SoulSifterSettings::getInstance().get<string>("music.dir") + song->getFilepath();
    if (!boost::filesystem::exists(songFilepath)) {
      songFilepath = song->getFilepath();
      if (!boost::filesystem::exists(songFilepath)) {
        LOG(WARNING) << "Unable to save id3v2 tag. File does not exist: " << song->getFilepath();
        return;
      }
    }

    TagLib::MPEG::File f(songFilepath.c_str());
    TagLib::ID3v2::Tag* id3v2 = f.ID3v2Tag(true);
    f.strip(TagLib::MPEG::File::ID3v1);
    id3v2->setArtist(song->getArtist());
    setId3v2Text(id3v2, "TRCK", song->getTrack().c_str());
    id3v2->setTitle(song->getTitle());
    setId3v2Text(id3v2, "TPE4", song->getRemixer().c_str());
    id3v2->setComment(song->getComments());
    setId3v2Text(id3v2, "TPE2", song->getAlbum()->getArtist().c_str());
    id3v2->setAlbum(song->getAlbum()->getName());
    setId3v2Text(id3v2, "TPUB", song->getAlbum()->getLabel().c_str());
    setId3v2Text(id3v2, "TCID", song->getAlbum()->getCatalogId().c_str());
    id3v2->setYear(song->getAlbum()->getReleaseDateYear());
    // set key (currently we will ignore if we have multiple. TODO fix?)
    if (!song->getTonicKey().empty()) setId3v2Text(id3v2, "TKEY", song->getTonicKey().c_str());
    // set genre
    for (long i = song->getStyles().size(); i > 0; --i) {
      const char* style = song->getStyles().back()->getName().c_str();
      if (style[0] != '=' && style[0] != '_' && style[0] != '.') {
        setId3v2Text(id3v2, "TCON", style);
        break;
      }
    }
    // set rating
    {
      id3v2->removeFrames("POPM");
      TagLib::ID3v2::PopularimeterFrame *frame = new TagLib::ID3v2::PopularimeterFrame();
      frame->setRating(song->getRating());
      id3v2->addFrame(frame);
    }
    // part of set
    if (song->getAlbumPart()) {
      setId3v2Text(id3v2, "TPOS", song->getAlbumPart()->getPos().c_str());
    } else {
      id3v2->removeFrames("TPOS");
    }
    // set release day & month
    if (song->getAlbum()->getReleaseDateMonth() > 0) {
      ostringstream daymonth;
      if (song->getAlbum()->getReleaseDateDay() == 0) {
        daymonth << "00";
      } else if (song->getAlbum()->getReleaseDateDay() < 10) {
        daymonth << "0" << song->getAlbum()->getReleaseDateDay();
      } else {
        daymonth << song->getAlbum()->getReleaseDateDay();
      }
      if (song->getAlbum()->getReleaseDateMonth() < 10) {
        daymonth << "0" << song->getAlbum()->getReleaseDateMonth();
      } else {
        daymonth << song->getAlbum()->getReleaseDateMonth();
      }
      setId3v2Text(id3v2, "TDAT", daymonth.str().c_str());
    }
    // custom tags
    if (song->getLowQuality()) {
      setId3v2UserText(id3v2, LOW_QUALITY, "1");
    } else {
      TagLib::ID3v2::UserTextIdentificationFrame* frame = TagLib::ID3v2::UserTextIdentificationFrame::find(id3v2, LOW_QUALITY);
      if (frame) id3v2->removeFrames(frame->frameID());
    }
    if (song->getCurator().length() > 0) {
      setId3v2UserText(id3v2, CURATOR, song->getCurator().c_str());
    } else {
      TagLib::ID3v2::UserTextIdentificationFrame* frame = TagLib::ID3v2::UserTextIdentificationFrame::find(id3v2, CURATOR);
      if (frame) id3v2->removeFrames(frame->frameID());
    }
    if (song->getYoutubeId().length() > 0) {
      setId3v2UserText(id3v2, YOUTUBE, song->getYoutubeId().c_str());
    } else {
      TagLib::ID3v2::UserTextIdentificationFrame* frame = TagLib::ID3v2::UserTextIdentificationFrame::find(id3v2, YOUTUBE);
      if (frame) id3v2->removeFrames(frame->frameID());
    }
    // picture
    setId3v2Picture(id3v2, SoulSifterSettings::getInstance().get<string>("music.dir") + song->getAlbum()->getCoverFilepath(), false);
    // save
    bool result = f.save();
    if (!result) {
      LOG(WARNING) << "unable to save id3v2 tag for " << song->getFilepath();
    } else {
      LOG(INFO) << "successfully wrote id3v2 tag for " << song->getFilepath();
    }
  }
}

void TagService::updateSongAttributesFromTags(std::function<void(float)> progressCallback) {
  LOG(INFO) << "updating song attributes from tags";

  // get max id
  vector<Style*> emptyStyles;
  vector<Song*> emptySongs;
  string query = "id:\"(select max(id) from songs)\"";
  vector<Song*>* songs = SearchUtil::searchSongs(query, 0, "", emptyStyles, emptySongs, 1);
  int maxId = 0;
  for (Song* song : *songs) {
    maxId = song->getId();
  }
  deleteVectorPointers(songs);

  // loop through songs
  int span = 100;
  int callbackBreakpoint = span * ceil((maxId / /* num of updates */ 50) / span);
  for (int i = 0; i <= maxId; i += span) {
    if (i % callbackBreakpoint == 0) progressCallback((float) i / maxId);
    stringstream ss;
    ss << "q:\"s.id >= " << i << "\" q:\"s.id < " << (i + span) << "\"";
    ss << " trashed:0";  // q:\"bpm is null\"";
    query = ss.str();
    songs = SearchUtil::searchSongs(query,  0, "", emptyStyles, emptySongs, span);

    for (Song* song : *songs) {
      if (readId3v2TagAttributes(song)) {
        LOG(INFO) << "updating song " << song->getId() << "'s attributes";
        song->update();
      }
    }
    deleteVectorPointers(songs);
  }
  progressCallback(1);
}
    
}  // namespace soulsifter
}  // namespace dogatech
