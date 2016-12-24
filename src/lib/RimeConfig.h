#pragma once

#include <QHash>

#include <yaml-cpp/yaml.h>

namespace fcitx_rime {
  class RimeConfigData : QObject {
  public:
    RimeConfigData() = default;
    ~RimeConfigData();
    bool LoadFromFile(const QString& file_name);
    bool SaveToFle(const QString& file_name);
  private:
    QString config_file_name;
  };
}
