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
    boolean firstRun;
  } FcitxRime;
  
  FcitxRime* FcitxRimeConfigCreate();
  void FcitxRimeConfigStart(FcitxRime* rime);

  void FcitxRimeConfigOpen(FcitxRime* rime);
  void FcitxRimeConfigReload();
  

#ifdef __cplusplus
}
#endif
