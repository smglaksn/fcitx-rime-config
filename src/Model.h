#pragma once
#include <QHash>

#include <fcitx-config/fcitx-config.h>
#include <rime_api.h>

namespace fcitx_rime {
  class FcitxRimeConfigDataModel : public QObject {
    Q_OBJECT
    public:
    int candidate_per_word;
    private:
    
  };
}

