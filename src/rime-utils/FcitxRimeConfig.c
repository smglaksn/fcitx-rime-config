#include "FcitxRimeConfig.h"
#include "assert.h"
#include "fcitx-config/xdg.h"
#include "fcitx-utils/utils.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

  FcitxRime* FcitxRimeConfigCreate() {
    FcitxRime* rime = (FcitxRime*) fcitx_utils_malloc0(sizeof(FcitxRime));
    rime->api = rime_get_api();
    rime->firstRun = True;
    rime->default_conf = NULL;
    if (!rime->api) {
      free(rime);
      return NULL;
    }
    return rime;
  }

  void FcitxRimeConfigStart(FcitxRime* rime) {
    char* user_path = NULL;
    FILE* fp = FcitxXDGGetFileUserWithPrefix(FCITX_RIME_DIR_PREFIX, ".place_holder", "w", NULL);
    if (fp) {
      fclose(fp);
    }
    FcitxXDGGetFileUserWithPrefix(FCITX_RIME_DIR_PREFIX, "", NULL, &user_path);
    const char* shared_data_dir = fcitx_utils_get_fcitx_path_with_filename("pkgdatadir", FCITX_RIME_DIR_PREFIX);
    RIME_STRUCT(RimeTraits, fcitx_rime_traits);
    fcitx_rime_traits.shared_data_dir = shared_data_dir;
    fcitx_rime_traits.app_name = "rime.fcitx-rime-config";
    fcitx_rime_traits.user_data_dir = user_path;
    fcitx_rime_traits.distribution_name = "Rime";
    fcitx_rime_traits.distribution_code_name = "fcitx-rime-config";
    fcitx_rime_traits.distribution_version = "0.0.2";
    if(rime->firstRun) {
      rime->api->setup(&fcitx_rime_traits);
      rime->firstRun = False;
    }
    rime->api->initialize(&fcitx_rime_traits);
  }

  RimeConfig* FcitxRimeConfigOpenDefault(FcitxRime* rime) {
    RimeConfig* fcitx_rime_config_default = fcitx_utils_malloc0(sizeof(RimeConfig));
    Bool suc = rime->api->config_open("default", fcitx_rime_config_default);
    if (!suc) {
      return NULL;
    }
    rime->default_conf = fcitx_rime_config_default;
    return fcitx_rime_config_default;
  }
  
  void FcitxRimeConfigSetToggleKeys(FcitxRime* rime, RimeConfig* config, const char* key0, const char* key1) {
    size_t s = RimeConfigListSize(config, "switcher/hotkeys");
    RimeConfigIterator* iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));
    RimeConfigBeginList(iterator, config, "switcher/hotkeys");
    for(size_t i = 0; i < s; i ++) {
        RimeConfigNext(iterator);
        char key_rime[30];
        memset(key_rime, 0, sizeof(key_rime));
        if(i == 0) {
          RimeConfigSetString(config, iterator->path, key0);
        } else if (i == 1) {
          RimeConfigSetString(config, iterator->path, key1);
        } else {
          RimeConfigSetString(config, iterator->path, "");
        }
    }
    fcitx_utils_free(iterator);
  }
  
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config, char** keys, size_t keys_size) {
    size_t s = RimeConfigListSize(config, "switcher/hotkeys");
    RimeConfigIterator* iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));
    RimeConfigBeginList(iterator, config, "switcher/hotkeys");
    for(size_t i = 0; i < s; i ++) {
        RimeConfigNext(iterator);
        if (i >= keys_size) {
          RimeConfigEnd(iterator);
          break;
        } else {
          char* mem = fcitx_utils_malloc0(30);
          RimeConfigGetString(config, iterator->path, mem, 30); 
          keys[i] = mem;
        }
    }
    fcitx_utils_free(iterator);
  }
  
  
  
  // Get key bindings 
  static RimeConfigIterator* global_iterator;
  void FcitxRimeBeginKeyBinding(RimeConfig* config) {
    global_iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));  
    RimeConfigBeginList(global_iterator, config, "key_binder/bindings");
  }
  
  size_t FcitxRimeConfigGetKeyBindingSize(RimeConfig *config) {
    return RimeConfigListSize(config, "key_binder/bindings");
  }
  
  void FcitxRimeEndKeyBinding(RimeConfig* config) {
    fcitx_utils_free(global_iterator);
  }
  
  void FcitXRimeConfigGetNextKeyBinding(RimeConfig* config, char* act_type, char* act_key, char* accept, size_t buffer_size) {
    RimeConfigNext(global_iterator);
    RimeConfig* map = fcitx_utils_malloc0(sizeof(RimeConfig));
    RimeConfigGetItem(config, global_iterator->path, map);
    
    char* accept_try = fcitx_utils_malloc0(buffer_size * sizeof(char));
    memset(accept_try, 0 , buffer_size * sizeof(char));
    RimeConfigGetString(map, "accept", accept_try, buffer_size);
    
    char* send_try = fcitx_utils_malloc0(buffer_size * sizeof(char));
    memset(send_try, 0 , buffer_size * sizeof(char));
    RimeConfigGetString(map, "send", send_try, buffer_size);
    
    char* toggle_try = fcitx_utils_malloc0(buffer_size * sizeof(char));
    memset(toggle_try, 0 , buffer_size * sizeof(char));
    RimeConfigGetString(map, "toggle", toggle_try, buffer_size);
    
    if(strlen(send_try) != 0) {
      strncpy(act_type, "send", buffer_size);
      strncpy(act_key, send_try, buffer_size);
    } else if (strlen(toggle_try) != 0) {
      strncpy(act_type, "toggle", buffer_size);
      strncpy(act_key, toggle_try, buffer_size);
    }
    
    strncpy(accept, accept_try, buffer_size);
    
    // free things
    fcitx_utils_free(accept_try);
    fcitx_utils_free(send_try);
    fcitx_utils_free(toggle_try);
    fcitx_utils_free(map);
  }

  void FcitxRimeDestroy(FcitxRime* rime) {
    RimeConfigClose(rime->default_conf);
    rime->api->finalize();
    fcitx_utils_free(rime);
  }
  
  // Write into config file and restart rime
  void FcitxRimeConfigSync(FcitxRime* rime) {
    RimeStartMaintenanceOnWorkspaceChange();
    RimeConfigClose(rime->default_conf);
    rime->api->finalize();
    FcitxRimeConfigStart(rime);
    FcitxRimeConfigOpenDefault(rime);
  }
  
  void FcitxRimeGetSchemaAttr(FcitxRime* rime, const char* schema_id, char* name, size_t buffer_size, const char* attr) {
    RimeConfig* rime_schema_config = fcitx_utils_malloc0(sizeof(RimeConfig));
    RimeSchemaOpen(schema_id, rime_schema_config);
    RimeConfigGetString(rime_schema_config, attr, name, buffer_size);
    RimeConfigClose(rime_schema_config);
    fcitx_utils_free(rime_schema_config);
  }
  
  int FcitxRimeCheckSchemaEnabled(FcitxRime* rime, RimeConfig* config, const char* schema_id) {
    size_t s = RimeConfigListSize(config, "schema_list");
    RimeConfigIterator *iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));
    RimeConfigBeginList(iterator, config, "schema_list");
    int result = 0;
    for(size_t i = 0; i < s; i ++) {
      RimeConfigNext(iterator);
      RimeConfig* map = fcitx_utils_malloc0(sizeof(RimeConfig));
      RimeConfigGetItem(config, iterator->path, map);
      size_t buffer_size = 50;
      char* s = fcitx_utils_malloc0(buffer_size * sizeof(char));
      RimeConfigGetString(map, "schema", s, buffer_size);
      if (strcmp(s, schema_id) == 0) { /* This schema is enabled in default*/        
        result = (i+1);
      }
      fcitx_utils_free(map);
    }
    fcitx_utils_free(iterator);
    return result;
  }
  
#ifdef __cplusplus
}
#endif
