// Author: Xu Zhao (i@xuzhao.net)
//

#pragma once

#include <string>
#include <yaml-cpp/yaml.h>

using namespace std;

class RimeConfig {
 public:
  RimeConfig(string conf_path);
 private:
  string m_conf_path;
};
