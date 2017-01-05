#pragma once
#include <fcitx-qt/fcitxqtconfiguiplugin.h>

class FcitxRimeConfigTool : public FcitxQtConfigUIPlugin {
    Q_OBJECT
public:
    Q_PLUGIN_METADATA(IID FcitxQtConfigUIFactoryInterface_iid FILE "fcitx-rime-config.json")
    explicit FcitxRimeConfigTool(QObject* parent = 0);
    virtual QString name();
    virtual QStringList files();
    virtual QString domain();
    virtual FcitxQtConfigUIWidget* create(const QString& key);
};
