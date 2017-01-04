#pragma once
#include <QHash>

#include <fcitx-config/fcitx-config.h>
#include <rime_api.h>

#include "fcitx-utils/keysym.h"
#include <qkeysequence.h>

namespace fcitx_rime {
  class FcitxKeySeq {
  public:
    FcitxKeyState state;
    int sym;
    FcitxKeySeq();
    FcitxKeySeq(const char* keyseq);
    FcitxKeySeq(const QKeySequence qkey);
    char* toString();
  };
  class FcitxRimeConfigDataModel : public QObject {
    Q_OBJECT
    public:
    FcitxKeySeq toggle_key0;
    FcitxKeySeq toggle_key1;
    int candidate_per_word;
    private:
    
  };
}

