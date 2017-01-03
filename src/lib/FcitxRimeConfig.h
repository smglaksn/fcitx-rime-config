#pragma once

#include <fcitx/instance.h>
#include <fcitx/context.h>
#include <fcitx/candidate.h>
#include <fcitx/hook.h>
#include <fcitx-config/xdg.h>
#include <rime_api.h>

#ifdef __cplusplus
extern "C" {
#endif

  typedef struct _FcitxRime {
    RimeApi* api;
    RimeConfig* default_conf;
    boolean firstRun;
  } FcitxRime;
  
  FcitxRime* FcitxRimeConfigCreate();
  void FcitxRimeConfigStart(FcitxRime* rime);

  RimeConfig* FcitxRimeConfigOpenDefault(FcitxRime* rime);
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config, char** keys, size_t keys_size);
  void FcitxRimeConfigSync(FcitxRime* rime);
  void FcitxRimeDestroy(FcitxRime* rime);
  void FcitxRimeKeySeqToFcitxKeySeq(char* rime_key, char* fcitx_key);
  void FcitxKeySeqToRimeKeySeq(char* fcitx_key, char* rime_key);

#ifdef __cplusplus
}
#endif

