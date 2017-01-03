#pragma once
#include <QHash>

#include <fcitx-config/fcitx-config.h>
#include <rime_api.h>

namespace fcitx_rime {
  class FcitxRimeConfigDataModel : public QObject {
    Q_OBJECT
    public:
    FcitxRimeConfigDataModel() {
        memset(toggle_key0, 0, sizeof(toggle_key0));
        memset(toggle_key1, 0, sizeof(toggle_key1));
    }
    char toggle_key0[30];
    char toggle_key1[30];
    int candidate_per_word;
    private:
    
  };
}

