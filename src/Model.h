#pragma once

#include <rime_api.h>

#include "fcitx-utils/flags.h"
#include "fcitx-utils/keysym.h"
#include "fcitx-utils/keysymgen.h"
#include <qkeysequence.h>

namespace fcitx_rime {
  typedef fcitx::Flags<fcitx::KeyState> KeyStates;
  typedef FcitxKeySym KeySym;
  class FcitxKeySeq {
  public:
    KeyStates states_;
    KeySym sym_;
    FcitxKeySeq();
    FcitxKeySeq(const char* keyseq);
    FcitxKeySeq(const QKeySequence qkey);
    std::string toString();
    std::string keySymToString(KeySym sym);
    KeySym keySymFromString(const char* keyString);
    KeySym keySymFromUnicode(uint32_t wc);    
  };
  
  class FcitxRimeSchema {
  public:
    QString path;
    QString id;
    QString name;
    int index; // index starts from 1, 0 means not enabled
    bool active;
  };  
  
  class FcitxRimeConfigDataModel {
    public:
    FcitxKeySeq toggle_key0;
    FcitxKeySeq toggle_key1;
    FcitxKeySeq ascii_key;
    FcitxKeySeq trasim_key;
    FcitxKeySeq halffull_key;
    FcitxKeySeq pgup_key;
    FcitxKeySeq pgdown_key;
    int candidate_per_word;
    QVector<FcitxRimeSchema> schemas_;
    private:
    
  };
}

