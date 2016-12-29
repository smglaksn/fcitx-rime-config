#pragma once
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QHBoxLayout>

#include <fcitx-qt/fcitxqtconfiguiwidget.h>
#include <fcitx-qt/fcitxqtkeysequencewidget.h>

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
    void createGeneralWidget();
    QString addon();
    QString icon();
  private:
    Ui::MainUI* m_ui;
    FcitxRime* rime;
    void test();
  };
  
}
