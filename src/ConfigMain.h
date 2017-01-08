#pragma once

#include <fcitxqtconfiguiwidget.h>
#include <fcitxqtkeysequencewidget.h>

#include "Model.h"
#include "rime-utils/FcitxRimeConfig.h"

namespace Ui {
  class MainUI;
}

namespace fcitx_rime {
  
  class ConfigMain : public FcitxQtConfigUIWidget {
    Q_OBJECT
  public:
    explicit ConfigMain(QWidget* parent = 0);
    QString title();
    ~ConfigMain();
    void load();
    void save();
    
    QString addon();
    QString icon();
  public slots:
    void keytoggleChanged();
    void stateChanged();
    void addIM();
    void removeIM();
    void moveUpIM();
    void moveDownIM();
    void aboutIM();
    void availIMSelectionChanged();
    void activeIMSelectionChanged();
  private:
    Ui::MainUI* m_ui;
    FcitxRime* rime;
    FcitxRimeConfigDataModel* model;
    void test();
    void setFcitxQtKeySeq(char* rime_key, FcitxKeySeq& keyseq);
    void yamlToModel();
    void uiToModel();
    void modelToUi();
    void modelToYaml();
    void getAvailableSchemas();
    void updateIMList();
  };
  
}
