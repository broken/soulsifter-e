#include <napi.h>
#include "Song_wrap.h"
#include "Album.h"
#include "AlbumPart.h"
#include "AlbumPart_wrap.h"
#include "Album_wrap.h"
#include "MusicVideo.h"
#include "MusicVideo_wrap.h"
#include "ResultSetIterator.h"
#include "Song.h"
#include "Song_wrap.h"
#include "Style.h"
#include "Style_wrap.h"

Napi::FunctionReference* Song::constructor = nullptr;

Song::~Song() { if (ownWrappedObject) delete song; };

void Song::setWrappedValue(dogatech::soulsifter::Song* v, bool own) {
  if (ownWrappedObject)
    delete song;
  song = v;
  ownWrappedObject = own;
}

Napi::Object Song::Init(Napi::Env env, Napi::Object exports) {
  Napi::Function func = DefineClass(env, "Song", {
    InstanceMethod<&Song::clear>("clear"),
    StaticMethod<&Song::findById>("findById"),
    StaticMethod<&Song::findByFilepath>("findByFilepath"),
    StaticMethod<&Song::findByGoogleSongId>("findByGoogleSongId"),
    StaticMethod<&Song::findByYoutubeId>("findByYoutubeId"),
    StaticMethod<&Song::findBySpotifyId>("findBySpotifyId"),
    StaticMethod<&Song::findByRESongId>("findByRESongId"),
    StaticMethod<&Song::findAll>("findAll"),
    InstanceMethod<&Song::update>("update"),
    InstanceMethod<&Song::save>("save"),
    InstanceMethod<&Song::sync>("sync"),
    // Unable to process createRESongFromSong
    InstanceMethod<&Song::reAlbum>("reAlbum"),
    InstanceAccessor<&Song::getDateAddedString>("dateAddedString"),
    InstanceMethod<&Song::setDateAddedToNow>("setDateAddedToNow"),
    InstanceAccessor<&Song::getId, &Song::setId>("id"),
    InstanceAccessor<&Song::getArtist, &Song::setArtist>("artist"),
    InstanceAccessor<&Song::getTrack, &Song::setTrack>("track"),
    InstanceAccessor<&Song::getTitle, &Song::setTitle>("title"),
    InstanceAccessor<&Song::getRemixer, &Song::setRemixer>("remixer"),
    InstanceAccessor<&Song::getFeaturing, &Song::setFeaturing>("featuring"),
    InstanceAccessor<&Song::getFilepath, &Song::setFilepath>("filepath"),
    InstanceAccessor<&Song::getRating, &Song::setRating>("rating"),
    InstanceAccessor<&Song::getDateAdded, &Song::setDateAdded>("dateAdded"),
    InstanceAccessor<&Song::getBpm, &Song::setBpm>("bpm"),
    InstanceAccessor<&Song::getTonicKey, &Song::setTonicKey>("tonicKey"),
    InstanceAccessor<&Song::getEnergy, &Song::setEnergy>("energy"),
    InstanceAccessor<&Song::getComments, &Song::setComments>("comments"),
    InstanceAccessor<&Song::getTrashed, &Song::setTrashed>("trashed"),
    InstanceAccessor<&Song::getLowQuality, &Song::setLowQuality>("lowQuality"),
    InstanceAccessor<&Song::getGoogleSongId, &Song::setGoogleSongId>("googleSongId"),
    InstanceAccessor<&Song::getYoutubeId, &Song::setYoutubeId>("youtubeId"),
    InstanceAccessor<&Song::getSpotifyId, &Song::setSpotifyId>("spotifyId"),
    InstanceAccessor<&Song::getDurationInMs, &Song::setDurationInMs>("durationInMs"),
    InstanceAccessor<&Song::getCurator, &Song::setCurator>("curator"),
    InstanceAccessor<&Song::getRESongId, &Song::setRESongId>("rESongId"),
    // Unable to process getRESong
    // Unable to process getRESongConst
    // Unable to process setRESong
    InstanceAccessor<&Song::getAlbumId, &Song::setAlbumId>("albumId"),
    InstanceAccessor<&Song::getAlbum, &Song::setAlbum>("album"),
    InstanceAccessor<&Song::getAlbumConst>("albumConst"),
    InstanceAccessor<&Song::getAlbumPartId, &Song::setAlbumPartId>("albumPartId"),
    InstanceAccessor<&Song::getAlbumPart, &Song::setAlbumPart>("albumPart"),
    InstanceAccessor<&Song::getAlbumPartConst>("albumPartConst"),
    InstanceAccessor<&Song::getMusicVideoId, &Song::setMusicVideoId>("musicVideoId"),
    InstanceAccessor<&Song::getMusicVideo, &Song::setMusicVideo>("musicVideo"),
    InstanceAccessor<&Song::getMusicVideoConst>("musicVideoConst"),
    InstanceAccessor<&Song::getStyleIds, &Song::setStyleIds>("styleIds"),
    InstanceAccessor<&Song::getStyles, &Song::setStyles>("styles"),
    InstanceAccessor<&Song::getBpmLock, &Song::setBpmLock>("bpmLock"),
    InstanceAccessor<&Song::getTonicKeyLock, &Song::setTonicKeyLock>("tonicKeyLock"),
  });

  constructor = new Napi::FunctionReference();
  *constructor = Napi::Persistent(func);

  exports.Set("Song", func);
  return exports;
}

Napi::Object Song::NewInstance(Napi::Env env) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = Song::constructor->New({});
  return scope.Escape(napi_value(obj)).ToObject();
}

Song::Song(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Song>(info), song(nullptr), ownWrappedObject(true) {
  if (info.Length()) {
    dogatech::soulsifter::Song* x = Napi::ObjectWrap<Song>::Unwrap(info[0].As<Napi::Object>())->getWrappedValue();
    song = new dogatech::soulsifter::Song(*x);
  } else {
    song = new dogatech::soulsifter::Song();
  }
}

void Song::clear(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  obj->song->clear();
}

Napi::Value Song::findById(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findById(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findByFilepath(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findByFilepath(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findByGoogleSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findByGoogleSongId(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findByYoutubeId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findByYoutubeId(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findBySpotifyId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  std::string a0(info[0].As<Napi::String>().Utf8Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findBySpotifyId(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findByRESongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return env.Null();
  }
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for info[0])").ThrowAsJavaScriptException();
    return env.Null();
  }
  int32_t a0(info[0].As<Napi::Number>().Int32Value());
  dogatech::soulsifter::Song* result =
      dogatech::soulsifter::Song::findByRESongId(a0);

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue(result, true);
    return instance;
  }
}

Napi::Value Song::findAll(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  dogatech::ResultSetIterator<dogatech::soulsifter::Song>* result =
      dogatech::soulsifter::Song::findAll();

  vector<dogatech::soulsifter::Song*>* v = result->toVector();
  Napi::Array a = Napi::Array::New(env, static_cast<int>(v->size()));
  for (int i = 0; i < (int) v->size(); i++) {
    Napi::Object instance = Song::NewInstance(env);
    Song* r = Napi::ObjectWrap<Song>::Unwrap(instance);
    r->setWrappedValue((*v)[i], true);
    a.Set(i, instance);
  }
  delete v;
  return a;
}

Napi::Value Song::update(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->update();

  return Napi::Number::New(env, result);
}

Napi::Value Song::save(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->save();

  return Napi::Number::New(env, result);
}

Napi::Value Song::sync(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  bool result =  obj->song->sync();

  return Napi::Boolean::New(env, result);
}

Napi::Value Song::reAlbum(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->reAlbum();

  return Napi::String::New(env, result);
}

Napi::Value Song::getDateAddedString(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getDateAddedString();

  return Napi::String::New(env, result);
}

void Song::setDateAddedToNow(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  obj->song->setDateAddedToNow();
}

Napi::Value Song::getId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getId();

  return Napi::Number::New(env, result);
}

void Song::setId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setId(a0);
}

Napi::Value Song::getArtist(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getArtist();

  return Napi::String::New(env, result);
}

void Song::setArtist(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setArtist(a0);
}

Napi::Value Song::getTrack(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getTrack();

  return Napi::String::New(env, result);
}

void Song::setTrack(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setTrack(a0);
}

Napi::Value Song::getTitle(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getTitle();

  return Napi::String::New(env, result);
}

void Song::setTitle(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setTitle(a0);
}

Napi::Value Song::getRemixer(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getRemixer();

  return Napi::String::New(env, result);
}

void Song::setRemixer(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setRemixer(a0);
}

Napi::Value Song::getFeaturing(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getFeaturing();

  return Napi::String::New(env, result);
}

void Song::setFeaturing(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setFeaturing(a0);
}

Napi::Value Song::getFilepath(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getFilepath();

  return Napi::String::New(env, result);
}

void Song::setFilepath(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setFilepath(a0);
}

Napi::Value Song::getRating(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getRating();

  return Napi::Number::New(env, result);
}

void Song::setRating(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setRating(a0);
}

Napi::Value Song::getDateAdded(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  time_t result =  obj->song->getDateAdded();

  return Napi::Number::New(env, result * 1000);
}

void Song::setDateAdded(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number/time expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  time_t a0(value.As<Napi::Number>().Int32Value() / 1000);
  obj->song->setDateAdded(a0);
}

Napi::Value Song::getBpm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getBpm();

  return Napi::String::New(env, result);
}

void Song::setBpm(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setBpm(a0);
}

Napi::Value Song::getTonicKey(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getTonicKey();

  return Napi::String::New(env, result);
}

void Song::setTonicKey(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setTonicKey(a0);
}

Napi::Value Song::getEnergy(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getEnergy();

  return Napi::Number::New(env, result);
}

void Song::setEnergy(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setEnergy(a0);
}

Napi::Value Song::getComments(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getComments();

  return Napi::String::New(env, result);
}

void Song::setComments(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setComments(a0);
}

Napi::Value Song::getTrashed(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  bool result =  obj->song->getTrashed();

  return Napi::Boolean::New(env, result);
}

void Song::setTrashed(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  bool a0(value.As<Napi::Boolean>().Value());
  obj->song->setTrashed(a0);
}

Napi::Value Song::getLowQuality(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  bool result =  obj->song->getLowQuality();

  return Napi::Boolean::New(env, result);
}

void Song::setLowQuality(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  bool a0(value.As<Napi::Boolean>().Value());
  obj->song->setLowQuality(a0);
}

Napi::Value Song::getGoogleSongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getGoogleSongId();

  return Napi::String::New(env, result);
}

void Song::setGoogleSongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setGoogleSongId(a0);
}

Napi::Value Song::getYoutubeId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getYoutubeId();

  return Napi::String::New(env, result);
}

void Song::setYoutubeId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setYoutubeId(a0);
}

Napi::Value Song::getSpotifyId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getSpotifyId();

  return Napi::String::New(env, result);
}

void Song::setSpotifyId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setSpotifyId(a0);
}

Napi::Value Song::getDurationInMs(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getDurationInMs();

  return Napi::Number::New(env, result);
}

void Song::setDurationInMs(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setDurationInMs(a0);
}

Napi::Value Song::getCurator(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const string result =  obj->song->getCurator();

  return Napi::String::New(env, result);
}

void Song::setCurator(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsString()) {
    Napi::TypeError::New(env, "TypeError: String expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  std::string a0(value.As<Napi::String>().Utf8Value());
  obj->song->setCurator(a0);
}

Napi::Value Song::getRESongId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getRESongId();

  return Napi::Number::New(env, result);
}

void Song::setRESongId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setRESongId(a0);
}

Napi::Value Song::getAlbumId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getAlbumId();

  return Napi::Number::New(env, result);
}

void Song::setAlbumId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setAlbumId(a0);
}

Napi::Value Song::getAlbum(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::Album* result =  obj->song->getAlbum();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Album::NewInstance(env);
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Song::getAlbumConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::Album* result =  obj->song->getAlbumConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = Album::NewInstance(env);
    Album* r = Napi::ObjectWrap<Album>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Song::setAlbum(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::Album* a0tmp(Napi::ObjectWrap<Album>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::Album& a0 = *a0tmp;
  obj->song->setAlbum(a0);
}

Napi::Value Song::getAlbumPartId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getAlbumPartId();

  return Napi::Number::New(env, result);
}

void Song::setAlbumPartId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setAlbumPartId(a0);
}

Napi::Value Song::getAlbumPart(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::AlbumPart* result =  obj->song->getAlbumPart();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = AlbumPart::NewInstance(env);
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Song::getAlbumPartConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::AlbumPart* result =  obj->song->getAlbumPartConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = AlbumPart::NewInstance(env);
    AlbumPart* r = Napi::ObjectWrap<AlbumPart>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Song::setAlbumPart(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::AlbumPart* a0tmp(Napi::ObjectWrap<AlbumPart>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::AlbumPart& a0 = *a0tmp;
  obj->song->setAlbumPart(a0);
}

Napi::Value Song::getMusicVideoId(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  int result =  obj->song->getMusicVideoId();

  return Napi::Number::New(env, result);
}

void Song::setMusicVideoId(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsNumber()) {
    Napi::TypeError::New(env, "TypeError: Number expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  int32_t a0(value.As<Napi::Number>().Int32Value());
  obj->song->setMusicVideoId(a0);
}

Napi::Value Song::getMusicVideo(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::MusicVideo* result =  obj->song->getMusicVideo();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = MusicVideo::NewInstance(env);
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

Napi::Value Song::getMusicVideoConst(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  dogatech::soulsifter::MusicVideo* result =  obj->song->getMusicVideoConst();

  if (result == NULL) {
    return env.Null();
  } else {
    Napi::Object instance = MusicVideo::NewInstance(env);
    MusicVideo* r = Napi::ObjectWrap<MusicVideo>::Unwrap(instance);
    r->setWrappedValue(result, false);
    return instance;
  }
}

void Song::setMusicVideo(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsObject()) {
    Napi::TypeError::New(env, "TypeError: Object expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  dogatech::soulsifter::MusicVideo* a0tmp(Napi::ObjectWrap<MusicVideo>::Unwrap(value.As<Napi::Object>())->getWrappedValue());
  dogatech::soulsifter::MusicVideo& a0 = *a0tmp;
  obj->song->setMusicVideo(a0);
}

Napi::Value Song::getStyleIds(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const std::vector<int>& result =  obj->song->getStyleIds();

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    a.Set(i, Napi::Number::New(env, result[i]));
  }
  return a;
}

void Song::setStyleIds(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  Napi::Array a0Array = value.As<Napi::Array>();
  std::vector<int> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    if (!a0Array.Get(i).IsNumber()) {
      Napi::TypeError::New(env, "TypeError: Number expected (for a0Array.Get(i))").ThrowAsJavaScriptException();
      return;
    }
    int32_t x(a0Array.Get(i).As<Napi::Number>().Int32Value());
    a0.push_back(x);
  }
  obj->song->setStyleIds(a0);
}

Napi::Value Song::getStyles(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  const std::vector<dogatech::soulsifter::Style*>& result =  obj->song->getStyles();

  Napi::Array a = Napi::Array::New(env, static_cast<int>(result.size()));
  for (int i = 0; i < (int) result.size(); i++) {
    Napi::Object instance = Style::NewInstance(env);
    Style* r = Napi::ObjectWrap<Style>::Unwrap(instance);
    r->setWrappedValue((result)[i], false);
    a.Set(i, instance);
  }
  return a;
}

void Song::setStyles(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsArray()) {
    Napi::TypeError::New(env, "TypeError: Array expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  Napi::Array a0Array = value.As<Napi::Array>();
  std::vector<dogatech::soulsifter::Style*> a0;
  for (uint32_t i = 0; i < a0Array.Length(); ++i) {
    if (!a0Array.Get(i).IsObject()) {
      Napi::TypeError::New(env, "TypeError: Object expected (for a0Array.Get(i))").ThrowAsJavaScriptException();
      return;
    }
    dogatech::soulsifter::Style* x(Napi::ObjectWrap<Style>::Unwrap(a0Array.Get(i).As<Napi::Object>())->getWrappedValue());
    a0.push_back(x);
  }
  obj->song->setStyles(a0);
}

Napi::Value Song::getBpmLock(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  bool result =  obj->song->getBpmLock();

  return Napi::Boolean::New(env, result);
}

void Song::setBpmLock(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  bool a0(value.As<Napi::Boolean>().Value());
  obj->song->setBpmLock(a0);
}

Napi::Value Song::getTonicKeyLock(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Song* obj = this;
  bool result =  obj->song->getTonicKeyLock();

  return Napi::Boolean::New(env, result);
}

void Song::setTonicKeyLock(const Napi::CallbackInfo& info, const Napi::Value &value) {
  Napi::Env env = info.Env();
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected at least 1 argument.").ThrowAsJavaScriptException();
    return;
  }
  Song* obj = this;
  if (!value.IsBoolean()) {
    Napi::TypeError::New(env, "TypeError: Boolean expected (for value)").ThrowAsJavaScriptException();
    return;
  }
  bool a0(value.As<Napi::Boolean>().Value());
  obj->song->setTonicKeyLock(a0);
}

