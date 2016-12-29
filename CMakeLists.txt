cmake_minimum_required(VERSION 2.8)
project(fcitx-rime-config-gui-tool)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

# Dependencies
find_package(Fcitx 4.2.8 REQUIRED)
find_package(Qt4 4.8 REQUIRED)
find_package(Rime REQUIRED)
pkg_check_modules(FCITX_QT "fcitx-qt>=4.2.8")
pkg_check_modules(RIME "rime>=1.0.0" REQUIRED)

add_subdirectory(src)
