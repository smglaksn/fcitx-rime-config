#pragma once
#include <fcitx-qt/fcitxqtconfiguiplugin.h>

class FcitxRimeConfigTool : public FcitxQtConfigUIPlugin {
    Q_OBJECT
public:
    explicit FcitxRimeConfigTool(QObject* parent = 0);
    virtual QString name();
    virtual QStringList files();
    virtual QString domain();
    virtual FcitxQtConfigUIWidget* create(const QString& key);
};
