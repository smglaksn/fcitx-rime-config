#pragma once

#include <rime_api.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _FcitxRime {
    RimeApi* api;
    RimeConfig* default_conf;
    Bool firstRun;
  } FcitxRime;
  
  // public apis
  FcitxRime* FcitxRimeConfigCreate();
  void FcitxRimeConfigStart(FcitxRime* rime);
  RimeConfig* FcitxRimeConfigOpenDefault(FcitxRime* rime);
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config, char** keys, size_t keys_size);
  void FcitxRimeConfigSetToggleKeys(FcitxRime* rime, RimeConfig* config, const char* key0, const char* key1);
  void FcitxRimeConfigSync(FcitxRime* rime);
  void FcitxRimeDestroy(FcitxRime* rime);

#ifdef __cplusplus
}
#endif

