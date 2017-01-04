#include "FcitxRimeConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

  FcitxRime* FcitxRimeConfigCreate() {
    FcitxRime* rime = (FcitxRime*) fcitx_utils_malloc0(sizeof(FcitxRime));
    rime->api = rime_get_api();
    rime->firstRun = true;
    rime->default_conf = NULL;
    if (!rime->api) {
      free(rime);
      return NULL;
    }
    return rime;
  }

  void FcitxRimeConfigStart(FcitxRime* rime) {
    char* user_path = NULL;
    FILE* fp = FcitxXDGGetFileUserWithPrefix("rime", ".place_holder", "w", NULL);
    if (fp) {
      fclose(fp);
    }
    FcitxXDGGetFileUserWithPrefix("rime", "", NULL, &user_path);
    const char* shared_data_dir = fcitx_utils_get_fcitx_path_with_filename("pkgdatadir", "rime");
    RIME_STRUCT(RimeTraits, fcitx_rime_traits);
    fcitx_rime_traits.shared_data_dir = shared_data_dir;
    fcitx_rime_traits.app_name = "rime.fcitx-rime-config";
    fcitx_rime_traits.user_data_dir = user_path;
    fcitx_rime_traits.distribution_name = "Rime";
    fcitx_rime_traits.distribution_code_name = "fcitx-rime-config";
    fcitx_rime_traits.distribution_version = "0.0.1";
    if(rime->firstRun) {
      rime->api->setup(&fcitx_rime_traits);
      rime->firstRun = false;
    }
    rime->api->initialize(&fcitx_rime_traits);
  }

  RimeConfig* FcitxRimeConfigOpenDefault(FcitxRime* rime) {
    RimeConfig* fcitx_rime_config_default = fcitx_utils_malloc0(sizeof(RimeConfig));
    Bool suc = rime->api->config_open("default", fcitx_rime_config_default);
    if (!suc) {
      // TODO: log in fcitx log dir
      fprintf(stderr, "[fcitx-rime-config] Cannot find default.yaml config file in RIME config dir.\n");
      return NULL;
    }
    rime->default_conf = fcitx_rime_config_default;
    return fcitx_rime_config_default;
  }
  
  void FcitxRimeConfigSetToggleKeys(FcitxRime* rime, RimeConfig* config, char* key0, char* key1) {
    size_t s = RimeConfigListSize(config, "switcher/hotkeys");
    RimeConfigIterator* iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));
    RimeConfigBeginList(iterator, config, "switcher/hotkeys");
    for(size_t i = 0; i < s; i ++) {
        RimeConfigNext(iterator);
        char key_rime[30];
        memset(key_rime, 0, sizeof(key_rime));
        if(i == 0) {
          printf("Saving key0 %s\n", key0);
          FcitxKeySeqToRimeKeySeq(key0, key_rime);
          printf("Converted %s\n", key_rime);
          RimeConfigSetString(config, iterator->path, key_rime);    
        } else if (i == 1) {
	  printf("Saving key1 %s\n", key1);
          FcitxKeySeqToRimeKeySeq(key1, key_rime);
	  printf("Converted %s\n", key_rime);
          RimeConfigSetString(config, iterator->path, key_rime);
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
        // TODO: find out maximum size of key sequence
        if (i >= keys_size) {
          // RimeConfigSetString(config, iterator->path, NULL);
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
  
  // Static functions
  
  void FcitxRimeKeySeqToFcitxKeySeq(char* rime_key, char* fcitx_key) {
    size_t index = 0;
    char temp[30];
    memset(temp, 0, sizeof(temp));
    size_t temp_index = 0;
    while(index <= strlen(rime_key)) {
        if(rime_key[index] == '+') {
            char* f_key = NULL;
            RimeToFcitxKeyMap(temp, &f_key);
            strcat(fcitx_key, f_key);
            strcat(fcitx_key, "+");
            memset(temp, 0, sizeof(temp));
            temp_index = 0;
        } else if (index == strlen(rime_key)) {
            char* f_key = NULL;
            RimeToFcitxKeyMap(temp, &f_key);
            strcat(fcitx_key, f_key);
        } else {
            temp[temp_index] = rime_key[index];
            temp_index ++;
        }
        index ++;
    }
    return;
  }
  
  void FcitxKeySeqToRimeKeySeq(char* fcitx_key, char* rime_key) {
    size_t index = 0;
    char temp[30];
    memset(temp, 0, sizeof(temp));
    size_t temp_index = 0;
    while(index <= strlen(fcitx_key)) {
        if(fcitx_key[index] == '+') {
            char* f_key = NULL;
            FcitxToRimeKeyMap(temp, &f_key);
            strcat(rime_key, f_key);
            strcat(rime_key, "+");
            memset(temp, 0, sizeof(temp));
            temp_index = 0;
        } else if (index == strlen(fcitx_key)) {
            char* f_key = NULL;
            FcitxToRimeKeyMap(temp, &f_key);
            strcat(rime_key, f_key);
        } else {
            temp[temp_index] = fcitx_key[index];
            temp_index ++;
        }
        index ++;
    }
    return;    
  }
  
  static void RimeToFcitxKeyMap(char* rime_key, char** fcitx_key) {    
    if(strcmp(rime_key, "Control") == 0) {
        *fcitx_key = "Ctrl";
    } else if(strcmp(rime_key, "grave") == 0) {
        *fcitx_key = "`";
    } else {
        *fcitx_key = rime_key;
    }
  }
  
  static void FcitxToRimeKeyMap(char* fcitx_key, char** rime_key) {
    printf("Changing %s\n", fcitx_key);
    if(strcmp(fcitx_key, "Ctrl") == 0) {
        *rime_key = "Control";
    } else if(strcmp(fcitx_key, "`") == 0) {
        *rime_key = "grave";
    } else {
        *rime_key = fcitx_key;
    }
  }
  
  
#ifdef __cplusplus
}
#endif
