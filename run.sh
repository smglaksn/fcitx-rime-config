#!/usr/bin/env bash
mkdir build
cd build/
cmake ..
make && sudo make install && fcitx-qt-gui-wrapper rime/config
