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
    // Setup UI
    this->setMinimumSize(500, 500);
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
    // connections
    connect(m_ui->cand_cnt_spinbox, SIGNAL(valueChanged(int)), 
            this, SLOT(stateChanged()));
    connect(m_ui->toggle_shortcut, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->toggle_shortcut_2, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->hotkey_pageup, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->hotkey_pageup_2, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->hotkey_pagedown, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->hotkey_pagedown_2, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->shortcut_ascii, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->transim_shortcut, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->hotkey_hfshape, SIGNAL(keySequenceChanged(QKeySequence, FcitxQtModifierSide)), 
            this, SLOT(keytoggleChanged()));
    connect(m_ui->removeIMButton, SIGNAL(clicked(bool)), this, SLOT(removeIM()));
    connect(m_ui->addIMButton, SIGNAL(clicked(bool)), this, SLOT(addIM()));
    connect(m_ui->availIMView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(availIMSelectionChanged()));
    connect(m_ui->currentIMView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)), this, SLOT(activeIMSelectionChanged()));
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
  
  // SLOTs
  void ConfigMain::stateChanged() {
    emit changed(true);
  }
  
  void ConfigMain::addIM() {
    if(m_ui->availIMView->currentIndex().isValid()) {
      const QString uniqueName =m_ui->availIMView->currentIndex().data(Qt::DisplayRole).toString();
      for(size_t i = 0; i < this->model->schemas_.size(); i ++) {
        if(this->model->schemas_[i].name == uniqueName) {
          this->model->schemas_[i].active = true;
          this->model->schemas_[i].index = this->model->schemas_.last().index + 1;
        }
      }
      qSort(this->model->schemas_.begin(), this->model->schemas_.end(), [](const FcitxRimeSchema& s1, const FcitxRimeSchema& s2) {
       if(s1.index == s2.index) {
          return s1.id < s2.id;
        } else {
          return s1.index < s2.index;}});
      this->updateIMList();
      this->stateChanged();
    }
  }
  
  void ConfigMain::removeIM() {
    if(m_ui->currentIMView->currentIndex().isValid()) {
      const QString uniqueName =m_ui->currentIMView->currentIndex().data(Qt::DisplayRole).toString();
      for(size_t i = 0; i < this->model->schemas_.size(); i ++) {
        if(this->model->schemas_[i].name == uniqueName) {
          this->model->schemas_[i].active = false;
          this->model->schemas_[i].index = 0;
        }
      }
      qSort(this->model->schemas_.begin(), this->model->schemas_.end(), [](const FcitxRimeSchema& s1, const FcitxRimeSchema& s2) {
        if(s1.index == s2.index) {
          return s1.id < s2.id;
        } else {
          return s1.index < s2.index;}});
      this->updateIMList();
      this->stateChanged();
    }
  }
  
  void ConfigMain::moveUpIM() {}
  
  void ConfigMain::moveDownIM() {}
  
  void ConfigMain::aboutIM() {}
  
  void ConfigMain::availIMSelectionChanged() {
    if(!m_ui->availIMView->currentIndex().isValid()) {
      m_ui->addIMButton->setEnabled(false);
    } else {
      m_ui->addIMButton->setEnabled(true);
    }
  }
  
  void ConfigMain::activeIMSelectionChanged() {
    if(!m_ui->currentIMView->currentIndex().isValid()) {
      m_ui->removeIMButton->setEnabled(false);
      m_ui->moveUpButton->setEnabled(false);
      m_ui->moveDownButton->setEnabled(false);
      m_ui->configureButton->setEnabled(false);
    } else {
      m_ui->removeIMButton->setEnabled(true);
      m_ui->configureButton->setEnabled(true);
      if(m_ui->currentIMView->currentIndex().row() == 0) {
        m_ui->moveUpButton->setEnabled(false);
      } else {
        m_ui->moveUpButton->setEnabled(true);
      }
      if(m_ui->currentIMView->currentIndex().row() == 
        m_ui->currentIMView->model()->rowCount() - 1)
      {
        m_ui->moveDownButton->setEnabled(false);
      } else {
        m_ui->moveDownButton->setEnabled(true);
      }
    }
  }
  // end of SLOTs
  
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
    QString pgup_key_acttype = model->pgup_key[0].acttype_;
    QString pgup_key_actkey = model->pgup_key[0].actkey_;
    QString pgdwn_key_acttype = model->pgdown_key[0].acttype_;
    QString pgdwn_key_actkey = model->pgdown_key[0].actkey_;
    QString ascii_key_acttype = model->ascii_key[0].acttype_;
    QString ascii_key_actkey = model->ascii_key[0].actkey_;
    QString trasim_key_acttype = model->trasim_key[0].acttype_;
    QString trasim_key_actkey = model->trasim_key[0].actkey_;
    QString halffull_key_acttype = model->halffull_key[0].acttype_;
    QString halffull_key_actkey = model->halffull_key[0].actkey_;
    model->pgup_key[0] = FcitxKeySeq(m_ui->hotkey_pageup->keySequence());
    model->pgup_key[0].actkey_ = pgup_key_actkey; model->pgup_key[0].acttype_ = pgup_key_acttype;
    model->pgup_key[1] = FcitxKeySeq(m_ui->hotkey_pageup_2->keySequence());
    model->pgup_key[1].actkey_ = pgup_key_actkey; model->pgup_key[1].acttype_ = pgup_key_acttype;
    model->pgdown_key[0] = FcitxKeySeq(m_ui->hotkey_pagedown->keySequence());
    model->pgdown_key[0].actkey_ = pgdwn_key_actkey; model->pgdown_key[0].acttype_ = pgdwn_key_acttype;
    model->pgdown_key[1] = FcitxKeySeq(m_ui->hotkey_pagedown_2->keySequence());
    model->pgdown_key[1].actkey_ = pgdwn_key_actkey; model->pgdown_key[1].acttype_ = pgdwn_key_acttype;
    model->ascii_key[0] = FcitxKeySeq(m_ui->shortcut_ascii->keySequence());
    model->ascii_key[0].actkey_ = ascii_key_actkey; model->ascii_key[0].acttype_ = ascii_key_acttype;
    model->trasim_key[0] = FcitxKeySeq(m_ui->transim_shortcut->keySequence());
    model->trasim_key[0].actkey_ = trasim_key_actkey; model->trasim_key[0].acttype_ = trasim_key_acttype;
    model->halffull_key[0] = FcitxKeySeq(m_ui->hotkey_hfshape->keySequence());
    model->halffull_key[0].actkey_ = halffull_key_actkey; model->halffull_key[0].acttype_ = halffull_key_acttype;
    // active schemas already in model so we don't need to save it to model
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
    m_ui->shortcut_ascii->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->ascii_key[0].sym_, model->ascii_key[0].states_)));
    m_ui->transim_shortcut->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->trasim_key[0].sym_, model->trasim_key[0].states_)));
    m_ui->hotkey_hfshape->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->halffull_key[0].sym_, model->halffull_key[0].states_)));
    m_ui->hotkey_pagedown->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->pgdown_key[0].sym_, model->pgdown_key[0].states_)));
    if(model->pgdown_key.size() > 1) {
      m_ui->hotkey_pagedown_2->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->pgdown_key[1].sym_, model->pgdown_key[1].states_)));
    }
    m_ui->hotkey_pageup->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->pgup_key[0].sym_, model->pgup_key[0].states_)));
    if(model->pgup_key.size() > 1) {
      m_ui->hotkey_pageup_2->setKeySequence(QKeySequence(FcitxQtKeySequenceWidget::keyFcitxToQt(model->pgup_key[1].sym_, model->pgup_key[1].states_)));
    }
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

  void ConfigMain::updateIMList() {
    auto avail_IMmodel = static_cast<QStandardItemModel*>(m_ui->availIMView->model());
    auto active_IMmodel = static_cast<QStandardItemModel*>(m_ui->currentIMView->model());
    avail_IMmodel->removeRows(0, avail_IMmodel->rowCount());
    active_IMmodel->removeRows(0, active_IMmodel->rowCount());
    for(size_t i = 0; i < model->schemas_.size(); i ++) {
      auto& schema = model->schemas_[i];
      if(schema.active) {
        QStandardItem* active_schema = new QStandardItem(schema.name);
        active_IMmodel->appendRow(active_schema);
      } else {
        QStandardItem* inactive_schema = new QStandardItem(schema.name);
        avail_IMmodel->appendRow(inactive_schema);
      }
    }
  }
  
  void ConfigMain::modelToYaml() {
    this->rime->api->config_set_int(this->rime->default_conf,
					       "menu/page_size", this->model->candidate_per_word);
    FcitxRimeConfigSetToggleKeys(this->rime, this->rime->default_conf, model->toggle_key0.toString().c_str(), model->toggle_key1.toString().c_str());
    // save active schemas to custom yaml
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->ascii_key[0].acttype_.toStdString().c_str(),
                                 model->ascii_key[0].actkey_.toStdString().c_str(), model->ascii_key[0].toString().c_str(), 0);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->trasim_key[0].acttype_.toStdString().c_str(),
                                 model->trasim_key[0].actkey_.toStdString().c_str(), model->trasim_key[0].toString().c_str(), 0);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->halffull_key[0].acttype_.toStdString().c_str(),
                                 model->halffull_key[0].actkey_.toStdString().c_str(), model->halffull_key[0].toString().c_str(), 0);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->pgup_key[0].acttype_.toStdString().c_str(),
                                 model->pgup_key[0].actkey_.toStdString().c_str(), model->pgup_key[0].toString().c_str(), 0);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->pgup_key[1].acttype_.toStdString().c_str(),
                                 model->pgup_key[1].actkey_.toStdString().c_str(), model->pgup_key[1].toString().c_str(), 1);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->pgdown_key[0].acttype_.toStdString().c_str(),
                                 model->pgdown_key[0].actkey_.toStdString().c_str(), model->pgdown_key[0].toString().c_str(), 0);
    FcitXRimeConfigSetKeyBinding(this->rime->default_conf, model->pgdown_key[1].acttype_.toStdString().c_str(),
                                 model->pgdown_key[1].actkey_.toStdString().c_str(), model->pgdown_key[1].toString().c_str(), 1);
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
    // load other shortcuts
    FcitxRimeBeginKeyBinding(rime->default_conf);
    size_t buffer_size = 50;
    char* accept = (char*)fcitx_utils_malloc0(buffer_size);
    char* act_key = (char*)fcitx_utils_malloc0(buffer_size);
    char* act_type = (char*)fcitx_utils_malloc0(buffer_size);
    FcitxRimeBeginKeyBinding(rime->default_conf);
    size_t toggle_length = FcitxRimeConfigGetKeyBindingSize(rime->default_conf);
    for(size_t i = 0; i < toggle_length; i ++) {
      memset(accept, 0, buffer_size);
      memset(act_key, 0, buffer_size);
      memset(act_type, 0, buffer_size);
      FcitXRimeConfigGetNextKeyBinding(rime->default_conf, act_type, act_key, accept, buffer_size);
      if(strcmp(act_key, "ascii_mode") == 0) {
        FcitxKeySeq seq = FcitxKeySeq(accept);
        seq.acttype_ = QString::fromLocal8Bit(act_type);
        seq.actkey_ = QString::fromLocal8Bit(act_key);
        model->ascii_key.push_back(seq);
      } else if (strcmp(act_key, "full_shape") == 0) {
        FcitxKeySeq seq = FcitxKeySeq(accept);
        seq.acttype_ = QString::fromLocal8Bit(act_type);
        seq.actkey_ = QString::fromLocal8Bit(act_key);
        model->halffull_key.push_back(seq);
      } else if (strcmp(act_key, "simplification") == 0) {
        FcitxKeySeq seq = FcitxKeySeq(accept);
        seq.acttype_ = QString::fromLocal8Bit(act_type);
        seq.actkey_ = QString::fromLocal8Bit(act_key);
        model->trasim_key.push_back(seq);
      } else if (strcmp(act_key, "Page_Up") == 0) {
        FcitxKeySeq seq = FcitxKeySeq(accept);
        seq.acttype_ = QString::fromLocal8Bit(act_type);
        seq.actkey_ = QString::fromLocal8Bit(act_key);
        model->pgup_key.push_back(seq);
      } else if (strcmp(act_key, "Page_Down") == 0) {
        FcitxKeySeq seq = FcitxKeySeq(accept);
        seq.acttype_ = QString::fromLocal8Bit(act_type);
        seq.actkey_ = QString::fromLocal8Bit(act_key);
        model->pgdown_key.push_back(seq);
      }
    }
    fcitx_utils_free(accept);
    fcitx_utils_free(act_key);
    fcitx_utils_free(act_type);
    FcitxRimeEndKeyBinding(rime->default_conf);
    
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
      schema.index = FcitxRimeCheckSchemaEnabled(rime, rime->default_conf, id);
      schema.active = (bool)schema.index;
      fcitx_utils_free(name);
      fcitx_utils_free(id);
      model->schemas_.push_back(schema);
    }
    fcitx_utils_free_string_hash_set(files);
    qSort(model->schemas_.begin(), model->schemas_.end(), [](const FcitxRimeSchema& s1, const FcitxRimeSchema& s2) {
      if(s1.index == s2.index) {
          return s1.id < s2.id;
        } else {
          return s1.index < s2.index;}});
  }
  
  void ConfigMain::setFcitxQtKeySeq(char* rime_key, FcitxKeySeq& keyseq) {
    // from state and sym number to Qt key seq
    keyseq = FcitxKeySeq(rime_key);
    fcitx_utils_free(rime_key);
  }

};
