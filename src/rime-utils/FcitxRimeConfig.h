#pragma once

#include <rime_api.h>

#define FCITX_RIME_DIR_PREFIX "rime"
#define FCITX_RIME_CONFIG_SUFFIX ".yaml"
#define FCITX_RIME_SCHEMA_SUFFIX ".schema.yaml"

#ifdef __cplusplus
extern "C" {
#endif
  
  typedef struct _FcitxRime {
    RimeApi* api;
    RimeConfig* default_conf;
    RimeConfig* custom_conf;
    Bool firstRun;
  } FcitxRime;
  
  // open, start and close
  FcitxRime* FcitxRimeConfigCreate();
  void FcitxRimeConfigStart(FcitxRime* rime);
  RimeConfig* FcitxRimeConfigOpenDefault(FcitxRime* rime);
  void FcitxRimeConfigSync(FcitxRime* rime);
  void FcitxRimeDestroy(FcitxRime* rime);
  // toggle
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config, char** keys, size_t keys_size);
  void FcitxRimeConfigSetToggleKeys(FcitxRime* rime, RimeConfig* config, const char* key0, const char* key1);
  // schemas
  void FcitxRimeGetSchemaAttr(FcitxRime* rime, const char* schema_id, char* name, size_t buffer_size, const char* attr);
  int FcitxRimeCheckSchemaEnabled(FcitxRime* rime, RimeConfig* config, const char* schema_id);
  void FcitxRimeWriteActiveSchemas(FcitxRime* rime, RimeConfig* config);

#ifdef __cplusplus
}
#endif

