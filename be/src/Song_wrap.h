#ifndef Song_wrap_h
#define Song_wrap_h

#include <napi.h>
#include "Song.h"

class Song : public Napi::ObjectWrap<Song> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  static Napi::Object NewInstance(Napi::Env env);
  Song(const Napi::CallbackInfo& info);
  ~Song();

  void setWrappedValue(dogatech::soulsifter::Song* v, bool own);
  dogatech::soulsifter::Song* getWrappedValue() const { return song; }

 private:
  Napi::Value clear(const Napi::CallbackInfo& info);
  static Napi::Value findById(const Napi::CallbackInfo& info);
  static Napi::Value findByFilepath(const Napi::CallbackInfo& info);
  static Napi::Value findByGoogleSongId(const Napi::CallbackInfo& info);
  static Napi::Value findByYoutubeId(const Napi::CallbackInfo& info);
  static Napi::Value findBySpotifyId(const Napi::CallbackInfo& info);
  static Napi::Value findByRESongId(const Napi::CallbackInfo& info);
  static Napi::Value findAll(const Napi::CallbackInfo& info);
  Napi::Value update(const Napi::CallbackInfo& info);
  Napi::Value save(const Napi::CallbackInfo& info);
  Napi::Value sync(const Napi::CallbackInfo& info);
  // Unable to process createRESongFromSong
  Napi::Value reAlbum(const Napi::CallbackInfo& info);
  Napi::Value getDateAddedString(const Napi::CallbackInfo& info);
  Napi::Value setDateAddedToNow(const Napi::CallbackInfo& info);
  Napi::Value getId(const Napi::CallbackInfo& info);
  void setId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getArtist(const Napi::CallbackInfo& info);
  void setArtist(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getTrack(const Napi::CallbackInfo& info);
  void setTrack(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getTitle(const Napi::CallbackInfo& info);
  void setTitle(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getRemixer(const Napi::CallbackInfo& info);
  void setRemixer(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getFeaturing(const Napi::CallbackInfo& info);
  void setFeaturing(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getFilepath(const Napi::CallbackInfo& info);
  void setFilepath(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getRating(const Napi::CallbackInfo& info);
  void setRating(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getDateAdded(const Napi::CallbackInfo& info);
  void setDateAdded(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getBpm(const Napi::CallbackInfo& info);
  void setBpm(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getTonicKey(const Napi::CallbackInfo& info);
  void setTonicKey(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getEnergy(const Napi::CallbackInfo& info);
  void setEnergy(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getComments(const Napi::CallbackInfo& info);
  void setComments(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getTrashed(const Napi::CallbackInfo& info);
  void setTrashed(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getLowQuality(const Napi::CallbackInfo& info);
  void setLowQuality(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getGoogleSongId(const Napi::CallbackInfo& info);
  void setGoogleSongId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getYoutubeId(const Napi::CallbackInfo& info);
  void setYoutubeId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getSpotifyId(const Napi::CallbackInfo& info);
  void setSpotifyId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getDurationInMs(const Napi::CallbackInfo& info);
  void setDurationInMs(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getCurator(const Napi::CallbackInfo& info);
  void setCurator(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getRESongId(const Napi::CallbackInfo& info);
  void setRESongId(const Napi::CallbackInfo& info, const Napi::Value &value);
  // Unable to process getRESong
  // Unable to process getRESongConst
  // Unable to process setRESong
  Napi::Value getAlbumId(const Napi::CallbackInfo& info);
  void setAlbumId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAlbum(const Napi::CallbackInfo& info);
  Napi::Value getAlbumConst(const Napi::CallbackInfo& info);
  void setAlbum(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAlbumPartId(const Napi::CallbackInfo& info);
  void setAlbumPartId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getAlbumPart(const Napi::CallbackInfo& info);
  Napi::Value getAlbumPartConst(const Napi::CallbackInfo& info);
  void setAlbumPart(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getMusicVideoId(const Napi::CallbackInfo& info);
  void setMusicVideoId(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getMusicVideo(const Napi::CallbackInfo& info);
  Napi::Value getMusicVideoConst(const Napi::CallbackInfo& info);
  void setMusicVideo(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getStyleIds(const Napi::CallbackInfo& info);
  void setStyleIds(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getStyles(const Napi::CallbackInfo& info);
  void setStyles(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getBpmLock(const Napi::CallbackInfo& info);
  void setBpmLock(const Napi::CallbackInfo& info, const Napi::Value &value);
  Napi::Value getTonicKeyLock(const Napi::CallbackInfo& info);
  void setTonicKeyLock(const Napi::CallbackInfo& info, const Napi::Value &value);

  static Napi::FunctionReference* constructor;
  dogatech::soulsifter::Song* song;
  bool ownWrappedObject;
};

#endif
