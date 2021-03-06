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
    QString acttype_;
    QString actkey_;
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
    QVector<FcitxKeySeq> ascii_key;
    QVector<FcitxKeySeq> trasim_key;
    QVector<FcitxKeySeq> halffull_key;
    QVector<FcitxKeySeq> pgup_key;
    QVector<FcitxKeySeq> pgdown_key;
    int candidate_per_word;
    QVector<FcitxRimeSchema> schemas_;
    private:
    
  };
}

