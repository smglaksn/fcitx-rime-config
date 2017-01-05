#include <fcitx-config/xdg.h>

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
    printf("running config\n");
    this->setMinimumSize(500, 500);
    m_ui->setupUi(this);
    m_ui->verticallayout_general->setAlignment(Qt::AlignTop);
    connect(m_ui->cand_cnt_spinbox, SIGNAL(valueChanged(int)), 
            this, SLOT(stateChanged()));
    connect(m_ui->toggle_shortcut, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->toggle_shortcut_2, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    this->rime = FcitxRimeConfigCreate();
    FcitxRimeConfigStart(this->rime);
    this->yamlToModel();
    this->modelToUi();
  }
  
  ConfigMain::~ConfigMain() {
    FcitxRimeDestroy(this->rime);
    delete model;
    delete m_ui;
  }
  
  void ConfigMain::keytoggleChanged() {
    stateChanged();
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
    model->toggle_key0 = FcitxKeySeq(m_ui->toggle_shortcut->keySequence());
    model->toggle_key1 = FcitxKeySeq(m_ui->toggle_shortcut_2->keySequence());
  }
  
  void ConfigMain::save() {
    uiToModel();
    QFutureWatcher<void>* futureWatcher = new QFutureWatcher<void>(this);
    futureWatcher->setFuture(QtConcurrent::run<void>(this, &ConfigMain::modelToYaml));
    connect(futureWatcher, SIGNAL(finished()), this, SIGNAL(saveFinished()));
  }
  
  void ConfigMain::modelToUi() {
    m_ui->cand_cnt_spinbox->setValue(this->model->candidate_per_word);
    m_ui->toggle_shortcut->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->toggle_key0.sym_, model->toggle_key0.states_)));
    m_ui->toggle_shortcut_2->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->toggle_key1.sym_, model->toggle_key1.states_)));
  }

  void ConfigMain::modelToYaml() {
    this->rime->api->config_set_int(this->rime->default_conf,
					       "menu/page_size", this->model->candidate_per_word);
    FcitxRimeConfigSetToggleKeys(this->rime, this->rime->default_conf, model->toggle_key0.toString().c_str(), model->toggle_key1.toString().c_str());
    FcitxRimeConfigSync(this->rime);
    return;
  }
  
  void ConfigMain::yamlToModel() {
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
    setFcitxQtKeySeq(keys[0], model->toggle_key0);
    setFcitxQtKeySeq(keys[1], model->toggle_key1);
    fcitx_utils_free(keys);
  }
  
  void ConfigMain::setFcitxQtKeySeq(char* rime_key, FcitxKeySeq& keyseq) {
    // from state and sym number to Qt key seq
    keyseq = FcitxKeySeq(rime_key);
    fcitx_utils_free(rime_key);
  }

};
