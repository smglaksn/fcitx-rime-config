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
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config, char** keys, int keys_size);
  void FcitxRimeConfigSync(FcitxRime* rime);
  void FcitxRimeDestroy(FcitxRime* rime);
  

#ifdef __cplusplus
}
#endif
