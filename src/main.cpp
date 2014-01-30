#include <QApplication>
#include <qplugin.h>
#include <fcitx-utils/utils.h>
#include <fcitx-qt/fcitxqtconfiguiplugin.h>

#include <fcitx-qt/fcitxqtconfiguiplugin.h>

#include "ConfigUtility.h"
#include "main.h"

FRCU::FRCU(QObject* parent) : FcitxQtConfigUIPlugin(parent){
}

FcitxQtConfigUIWidget* FRCU::create(const QString& key)
{
  Q_UNUSED(key);
  return new fcitx_rime::ConfigUtility;
}

QString FRCU::name(){
  return "rime-config-utility";
}

QStringList FRCU::files(){
  return QStringList("rime/config");
}

QString FRCU::domain(){
  return "fcitx-rime";
}

Q_EXPORT_PLUGIN2(fcitx_rime_config_utility, FRCU)
