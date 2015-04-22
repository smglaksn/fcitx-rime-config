#ifndef _MAIN_H_
#define _MAIN_H_

#include <fcitx-qt/fcitxqtconfiguiplugin.h>

class FRCU : public FcitxQtConfigUIPlugin {
    Q_OBJECT
public:
    explicit FRCU(QObject* parent = 0);
    virtual QString name();
    virtual QStringList files();
    virtual QString domain();
    virtual FcitxQtConfigUIWidget* create(const QString& key);
};

#endif
