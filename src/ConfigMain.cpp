#include <fcitx-config/xdg.h>

#include <fcitx-qt/fcitxqtkeysequencewidget.h>
#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QDialogButtonBox>

#include "ConfigMain.h"
#include "ui_ConfigMain.h"
#include "Common.h"

// TODO: when failed-read happens, disable ui
// TODO: when failed-save happense, disable ui and show reason

namespace fcitx_rime {
  ConfigMain::ConfigMain(QWidget* parent) :
    FcitxQtConfigUIWidget(parent), m_ui(new Ui::MainUI),
    model(new FcitxRimeConfigDataModel()) {
    this->setMinimumSize(500, 500);
    m_ui->setupUi(this);
    m_ui->verticallayout_general->setAlignment(Qt::AlignTop);
    connect(m_ui->cand_cnt_spinbox, SIGNAL(valueChanged(int)), 
            this, SLOT(stateChanged()));
    this->rime = FcitxRimeConfigCreate();
    FcitxRimeConfigStart(this->rime);
    this->loadDefaultConfigFromYaml();
    this->modelToUi();
    this->test();
  }
  
  ConfigMain::~ConfigMain() {
    FcitxRimeDestroy(this->rime);
    delete model;
    delete m_ui;
  }
  
  void ConfigMain::stateChanged() {
    emit changed(true);
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
  }
  
  void ConfigMain::uiToModel() {
    model->candidate_per_word = m_ui->cand_cnt_spinbox->value();
  }
  
  void ConfigMain::save() {
    uiToModel();
    QFutureWatcher<void>* futureWatcher = new QFutureWatcher<void>(this);
    futureWatcher->setFuture(QtConcurrent::run<void>(this, &ConfigMain::modelToYaml));
    connect(futureWatcher, SIGNAL(finished()), this, SIGNAL(saveFinished()));
  }
  
  void ConfigMain::modelToUi() {
    m_ui->cand_cnt_spinbox->setValue(this->model->candidate_per_word);
  }

  void ConfigMain::modelToYaml() {
    this->rime->api->config_set_int(this->rime->default_conf,
					       "menu/page_size", this->model->candidate_per_word);
    FcitxRimeConfigSync(this->rime);
    return;
  }
  
  void ConfigMain::loadDefaultConfigFromYaml() {
    FcitxRimeConfigOpenDefault(this->rime);
    // load page size
    int page_size = 0;
    bool suc = this->rime->api->config_get_int(this->rime->default_conf, "menu/page_size", &page_size);
    if(suc) {
      this->model->candidate_per_word = page_size;
    }
    // load toggle keys
    size_t keys_size = 2;
    char** keys = (char**)fcitx_utils_malloc0(sizeof(char*) * keys_size);
    FcitxRimeConfigGetToggleKeys(this->rime, this->rime->default_conf, keys, keys_size);
    setFcitxQtKeySeq(keys[0], m_ui->toggle_shortcut);
    setFcitxQtKeySeq(keys[1], m_ui->toggle_shortcut_2);
    fcitx_utils_free(keys);
  }
  
  void ConfigMain::setFcitxQtKeySeq(char* rime_key, FcitxQtKeySequenceWidget*& widget) {
    char* fcitx_key = (char*)fcitx_utils_malloc0(30);
    FcitxRimeKeySeqToFcitxKeySeq(rime_key, fcitx_key);
    QKeySequence qtkey(tr(fcitx_key));
    widget->setKeySequence(qtkey);
    fcitx_utils_free(fcitx_key);
    fcitx_utils_free(rime_key);
  }
  
  void ConfigMain::test() {
    
  }

}
;
