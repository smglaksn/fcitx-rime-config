#include <fcitx-config/xdg.h>

#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QDialogButtonBox>
#include <QTreeWidgetItem>

#include "ConfigMain.h"
#include "ui_ConfigMain.h"
#include "Common.h"
#include <QListWidgetItem>
#include <QStandardItemModel>

// TODO: when failed-read happens, disable ui
// TODO: when failed-save happense, disable ui and show reason

namespace fcitx_rime {
  ConfigMain::ConfigMain(QWidget* parent) :
    FcitxQtConfigUIWidget(parent), m_ui(new Ui::MainUI),
    model(new FcitxRimeConfigDataModel()) {
    this->setMinimumSize(500, 500);
    // Setup UI
    m_ui->setupUi(this);
    m_ui->verticallayout_general->setAlignment(Qt::AlignTop);
    m_ui->filterTextEdit->setPlaceholderText("Search Input Method");
     m_ui->addIMButton->setIcon(QIcon::fromTheme("go-next"));
    m_ui->removeIMButton->setIcon(QIcon::fromTheme("go-previous"));
    m_ui->moveUpButton->setIcon(QIcon::fromTheme("go-up"));
    m_ui->moveDownButton->setIcon(QIcon::fromTheme("go-down"));
    m_ui->configureButton->setIcon(QIcon::fromTheme("help-about"));
    // listViews for currentIM and availIM
    QStandardItemModel* listModel = new QStandardItemModel();
    m_ui->currentIMView->setModel(listModel);
    QStandardItemModel* availIMModel = new QStandardItemModel();
    m_ui->availIMView->setModel(availIMModel);
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
    // set available and enabled input methods
    for(size_t i = 0; i < model->schemas_.size(); i ++) {
      auto& schema = model->schemas_[i];
      if(schema.active) {
        QStandardItem* active_schema = new QStandardItem(schema.name);
        auto model = static_cast<QStandardItemModel*>(m_ui->currentIMView->model());
        model->appendRow(active_schema);
      } else {
        QStandardItem* inactive_schema = new QStandardItem(schema.name);
        auto model = static_cast<QStandardItemModel*>(m_ui->availIMView->model());
        model->appendRow(inactive_schema);
      }
    }
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
    // load available schemas
    getAvailableSchemas();
  }
  
  void ConfigMain::getAvailableSchemas() {
    const char* absolute_path = RimeGetUserDataDir();
    FcitxStringHashSet* files = FcitxXDGGetFiles(FCITX_RIME_DIR_PREFIX, NULL, FCITX_RIME_SCHEMA_SUFFIX);
    HASH_SORT(files, fcitx_utils_string_hash_set_compare);
    HASH_FOREACH(f, files, FcitxStringHashSet) {
      auto schema = FcitxRimeSchema();
      schema.path = QString::fromLocal8Bit(f->name).prepend(absolute_path);
      auto basefilename = QString::fromLocal8Bit(f->name).section(".",0,0);
      size_t buffer_size = 50;
      char* name = static_cast<char*>(fcitx_utils_malloc0(buffer_size));
      char* id = static_cast<char*>(fcitx_utils_malloc0(buffer_size));
      FcitxRimeGetSchemaAttr(rime, basefilename.toStdString().c_str(), name, buffer_size, "schema/name");
      FcitxRimeGetSchemaAttr(rime, basefilename.toStdString().c_str(), id, buffer_size, "schema/schema_id");
      schema.name = QString::fromLocal8Bit(name);
      schema.id = QString::fromLocal8Bit(id);
      schema.active = (bool)FcitxRimeCheckSchemaEnabled(rime, rime->default_conf, id);
      fcitx_utils_free(name);
      fcitx_utils_free(id);
      model->schemas_.push_back(schema);
    }
    fcitx_utils_free_string_hash_set(files);
  }
  
  void ConfigMain::setFcitxQtKeySeq(char* rime_key, FcitxKeySeq& keyseq) {
    // from state and sym number to Qt key seq
    keyseq = FcitxKeySeq(rime_key);
    fcitx_utils_free(rime_key);
  }

};
