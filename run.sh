#!/usr/bin/env bash

pushd ~/git/fcitx-qt5/ && ./run.sh
popd
mkdir build
cd build/
cmake ..
make && sudo make install && fcitx-qt5-gui-wrapper rime/config
