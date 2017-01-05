#include "fcitx-utils/keynametable.h"
#include "fcitx-utils/utf8.h"
#include "fcitx-utils/keydata.h"
#include "Model.h"
#include <fcitx-qt/fcitxqtkeysequencewidget.h>

namespace fcitx_rime {
  
  // default constructor
  FcitxKeySeq::FcitxKeySeq() {
  }
  
  // convert keyseq to state and sym
  FcitxKeySeq::FcitxKeySeq(const char* keyseq) {
    KeyStates states;
    const char *p = keyseq;
    const char *lastModifier = keyseq;
    const char *found = nullptr;
    // use macro to check modifiers
    #define _CHECK_MODIFIER(NAME, VALUE)                                                                                   \
    if ((found = strstr(p, NAME))) {                                                                                   \
        states |= fcitx::KeyState::VALUE;                                                                                     \
        if (found + strlen(NAME) > lastModifier) {                                                                     \
            lastModifier = found + strlen(NAME);                                                                       \
        }                                                                                                              \
    }

    _CHECK_MODIFIER("CTRL_", Ctrl)
    _CHECK_MODIFIER("Control+", Ctrl)
    _CHECK_MODIFIER("ALT_", Alt)
    _CHECK_MODIFIER("Alt+", Alt)
    _CHECK_MODIFIER("SHIFT_", Shift)
    _CHECK_MODIFIER("Shift+", Shift)
    _CHECK_MODIFIER("SUPER_", Super)
    _CHECK_MODIFIER("Super+", Super)
    
#undef _CHECK_MODIFIER
    this->sym_ = keySymFromString(lastModifier);
    this->states_ = states;
  }
  
  KeySym FcitxKeySeq::keySymFromString(const char* keyString) {
    auto value = std::lower_bound(
        keyValueByNameOffset, keyValueByNameOffset + FCITX_ARRAY_SIZE(keyValueByNameOffset), keyString,
        [](const uint32_t &idx, const std::string &str) { return keyNameList[&idx - keyValueByNameOffset] < str; });
    if (value != keyValueByNameOffset + FCITX_ARRAY_SIZE(keyValueByNameOffset) &&
        strcmp(keyString, keyNameList[value - keyValueByNameOffset]) == 0) {
        return static_cast<KeySym>(*value);
    }

    if (fcitx::utf8::length(keyString) == 1) {
        auto chr = fcitx::utf8::getCharValidated(keyString);
        if (chr > 0) {
            if (fcitx::utf8::charLength(keyString) == 1) {
                return static_cast<KeySym>(keyString[0]);
            } else {
                return keySymFromUnicode(chr);
            }
        }
    }

    return FcitxKey_None;
  }
  
  KeySym FcitxKeySeq::keySymFromUnicode(uint32_t wc) {
    int min = 0;
    int max = sizeof(gdk_unicode_to_keysym_tab) / sizeof(gdk_unicode_to_keysym_tab[0]) - 1;
    int mid;

    /* First check for Latin-1 characters (1:1 mapping) */
    if ((wc >= 0x0020 && wc <= 0x007e) || (wc >= 0x00a0 && wc <= 0x00ff))
        return static_cast<KeySym>(wc);

    /* Binary search in table */
    while (max >= min) {
        mid = (min + max) / 2;
        if (gdk_unicode_to_keysym_tab[mid].ucs < wc)
            min = mid + 1;
        else if (gdk_unicode_to_keysym_tab[mid].ucs > wc)
            max = mid - 1;
        else {
            /* found it */
            return static_cast<KeySym>(gdk_unicode_to_keysym_tab[mid].keysym);
        }
    }

    /*
    * No matching keysym value found, return Unicode value plus 0x01000000
    * (a convention introduced in the UTF-8 work on xterm).
    */
    return static_cast<KeySym>(wc | 0x01000000);
  }
  
  std::string FcitxKeySeq::keySymToString(KeySym sym) {
    const KeyNameOffsetByValue *result =
    std::lower_bound(keyNameOffsetByValue, keyNameOffsetByValue + FCITX_ARRAY_SIZE(keyNameOffsetByValue), sym,
                      [](const KeyNameOffsetByValue &item, KeySym key) { return item.sym < key; });
    if (result != keyNameOffsetByValue + FCITX_ARRAY_SIZE(keyNameOffsetByValue) && result->sym == sym) {
      return keyNameList[result->offset];
    }
    return std::string();
  }
  
  // convert QKeySequence to state and sym
  FcitxKeySeq::FcitxKeySeq(const QKeySequence qkey) {
    int sym = 0;
    uint states = 0;
    int qkeycode = static_cast<int>(qkey[0]);
    FcitxQtKeySequenceWidget::keyQtToFcitx(qkeycode, FcitxQtModifierSide::MS_Unknown, sym, states);
    this->sym_ = static_cast<FcitxKeySym>(sym);
    this->states_ = static_cast<fcitx::KeyState>(states);
  }
  
  // convert to Rime X11 style string
  std::string fcitx_rime::FcitxKeySeq::toString() {
      auto sym = sym_;
      if (sym == FcitxKey_None) {
        return std::string();
      }
      if (sym == FcitxKey_ISO_Left_Tab) {
        sym = FcitxKey_Tab;          
      }
      
      auto key = keySymToString(sym);

      if (key.empty()) {
        return std::string();
      }

      std::string str;
      
      #define _APPEND_MODIFIER_STRING(STR, VALUE)                                                                         \
        if (states_ & fcitx::KeyState::VALUE) {                                                                                   \
            str += STR;                                                                                                   \
      }
      
      _APPEND_MODIFIER_STRING("Control+", Ctrl)
      _APPEND_MODIFIER_STRING("Alt+", Alt)
      _APPEND_MODIFIER_STRING("Shift+", Shift)
      _APPEND_MODIFIER_STRING("Super+", Super)

      #undef _APPEND_MODIFIER_STRING
      str += key;
      return str;
  }
}
