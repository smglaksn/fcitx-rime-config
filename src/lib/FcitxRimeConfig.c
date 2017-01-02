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
      fprintf(stderr, "[fcitx-rime-config] Cannot find default config!\n");
      return NULL;
    }
    rime->default_conf = fcitx_rime_config_default;
    return fcitx_rime_config_default;
  }
  
  void FcitxRimeConfigGetToggleKeys(FcitxRime* rime, RimeConfig* config) {
    size_t s = RimeConfigListSize(config, "switcher/hotkeys");
    RimeConfigIterator* iterator = fcitx_utils_malloc0(sizeof(RimeConfigIterator));
    RimeConfigBeginList(iterator, config, "switcher/hotkeys");
    // printf("here, %s\n", iterator->list->GetValue());
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
  
#ifdef __cplusplus
}
#endif
