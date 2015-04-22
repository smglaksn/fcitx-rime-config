#include <fcitx-config/xdg.h>

#include <QFileDialog>
#include <QMessageBox>

#include "ConfigUtility.h"
#include "ui_FRCU.h"
#include "Common.h"

namespace fcitx_rime{
  ConfigUtility::ConfigUtility(QWidget* parent) :
    FcitxQtConfigUIWidget(parent), m_ui(new Ui::MainUI){
    m_ui->setupUi(this);
    setWindowTitle(_("Fcitx Rime Config Utility"));
  }
  ConfigUtility::~ConfigUtility(){
    delete m_ui;
  }
  QString ConfigUtility::icon(){
    return "fcitx-rime";
  }
  QString ConfigUtility::addon(){
    return "fcitx-rime";
  }
  QString ConfigUtility::title(){
    return _("Fcitx Rime Config Utility");
  }
  void ConfigUtility::load(){
  }
  void ConfigUtility::save(){
  }
}
