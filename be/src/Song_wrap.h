#ifndef Song_wrap_h
#define Song_wrap_h

#include <node.h>
#include <nan.h>
#include "Song.h"

class Song : public Nan::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);
  static v8::Local<v8::Object> NewInstance();

  void setNwcpValue(dogatech::soulsifter::Song* v, bool own);
  dogatech::soulsifter::Song* getNwcpValue() const { return song; }

 private:
  Song();
  explicit Song(dogatech::soulsifter::Song* song);
  ~Song();

  static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);

  static void clear(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findById(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByFilepath(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByGoogleSongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByYoutubeId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findBySpotifyId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findByRESongId(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void findAll(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void update(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void save(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void sync(const Nan::FunctionCallbackInfo<v8::Value>& info);
  // Unable to process createRESongFromSong
  static void reAlbum(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getDateAddedString(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setDateAddedToNow(const Nan::FunctionCallbackInfo<v8::Value>& info);
  static void getId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getArtist(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setArtist(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getTrack(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setTrack(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getTitle(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setTitle(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getRemixer(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setRemixer(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getFeaturing(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setFeaturing(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getFilepath(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setFilepath(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getRating(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setRating(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getDateAdded(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setDateAdded(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getBpm(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setBpm(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getTonicKey(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setTonicKey(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getEnergy(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setEnergy(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getComments(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setComments(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getTrashed(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setTrashed(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getLowQuality(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setLowQuality(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getGoogleSongId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setGoogleSongId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getYoutubeId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setYoutubeId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getSpotifyId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setSpotifyId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getDurationInMs(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setDurationInMs(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getCurator(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setCurator(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getRESongId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setRESongId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  // Unable to process getRESong
  // Unable to process getRESongConst
  // Unable to process setRESong
  static void getAlbumId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbumId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAlbum(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getAlbumConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbum(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAlbumPartId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbumPartId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getAlbumPart(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getAlbumPartConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setAlbumPart(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getMusicVideoId(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setMusicVideoId(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getMusicVideo(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void getMusicVideoConst(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setMusicVideo(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getStyleIds(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setStyleIds(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getStyles(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setStyles(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getBpmLock(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setBpmLock(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);
  static void getTonicKeyLock(v8::Local<v8::String> property, const Nan::PropertyCallbackInfo<v8::Value>& info);
  static void setTonicKeyLock(v8::Local<v8::String> property, v8::Local<v8::Value> value, const Nan::PropertyCallbackInfo<void>& info);

  static Nan::Persistent<v8::Function> constructor;
  dogatech::soulsifter::Song* song;
  bool ownWrappedObject;
};

#endif
