#include "FcitxRimeConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

  static FcitxRime* FcitxRimeConfigCreate() {
    FcitxRime* rime = (FcitxRime*) fcitx_utils_malloc0(sizeof(FcitxRime));
    rime->api = rime_get_api();
    rime->firstRun = true;
    if (!rime->api) {
      free(rime);
      return NULL;
    }
    return rime;
  }

  static void FcitxRimeStart(FcitxRime* rime) {
    char* user_path = NULL;
  }
  
#ifdef __cplusplus
}
#endif
