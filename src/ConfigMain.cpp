#include <fcitx-config/xdg.h>

#include <fcitx-qt/fcitxqtkeysequencewidget.h>

#include "ConfigMain.h"
#include "ui_ConfigMain.h"
#include "Common.h"

namespace fcitx_rime {
  ConfigMain::ConfigMain(QWidget* parent) :
    FcitxQtConfigUIWidget(parent), m_ui(new Ui::MainUI) {
    this->setMinimumSize(500, 500);
// createConfigOptionWidget(m_ui->MainUI->findChild<QHBoxLayout*>("horizontalLayout_general"));
    m_ui->setupUi(this);
  }
  ConfigMain::~ConfigMain() {
    delete m_ui;
  }
  void ConfigMain::createGeneralWidget() {
    QVBoxLayout* layout = new QVBoxLayout(this);
    FcitxQtKeySequenceWidget* keyseq1 = new FcitxQtKeySequenceWidget();
    layout->addWidget(keyseq1);
  }
  
  QString ConfigMain::icon() {
    return "fcitx-rime";
  }
  QString ConfigMain::addon() {
    return "fcitx-rime";
  }
  QString ConfigMain::title() {
    return _("Fcitx Rime Config GUI Tool");
  }
  void ConfigMain::load() {
    FILE *fp = NULL;
  }
  void ConfigMain::save() {
  }
}
