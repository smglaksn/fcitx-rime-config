#include <fcitx-qt/fcitxqtconfiguiwidget.h>

namespace Ui{
  class MainUI;
}

namespace fcitx_rime {
  class ConfigUtility : public FcitxQtConfigUIWidget {
    Q_OBJECT
  public:
    explicit ConfigUtility(QWidget* parent = 0);
    virtual ~ConfigUtility();
    virtual void load();
    virtual void save();
    virtual QString title();
    virtual QString addon();
    virtual QString icon();
  private:
    Ui::MainUI* m_ui;
  };
  
}
