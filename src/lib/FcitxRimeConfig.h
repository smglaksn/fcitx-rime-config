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

static FcitxRime* FcitxRimeConfigCreate();
static void FcitxRimeConfigStart();
static void FcitxRimeConfigReload();

#ifdef __cplusplus
}
#endif
