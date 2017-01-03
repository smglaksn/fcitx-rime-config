#pragma once
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QHBoxLayout>

#include <fcitx-qt/fcitxqtconfiguiwidget.h>
#include <fcitx-qt/fcitxqtkeysequencewidget.h>

#include "Model.h"
#include "lib/FcitxRimeConfig.h"

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
    void stateChanged();
  private:
    Ui::MainUI* m_ui;
    FcitxRime* rime;
    FcitxRimeConfigDataModel* model;
    void test();
    void loadDefaultConfigFromYaml();
    void setFcitxQtKeySeq(char* rime_key, FcitxQtKeySequenceWidget*& widget, char* model_keyseq);
    void uiToModel();
    void modelToUi();
    void modelToYaml();
  };
  
}
