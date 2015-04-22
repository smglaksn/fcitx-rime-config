cmake_minimum_required(VERSION 2.8)
project(fcitx-rime-configtool)

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-sign-compare -Wno-unused-parameter -fvisibility=hidden ${CMAKE_CXX_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "-Wl,--as-needed ${CMAKE_MODULE_LINKER_FLAGS}")

#debug
set(RUN_COMMAND "fcitx-qt-gui-wrapper")
set(PROJ_KEY "rime/config")

# Dependencies
find_package(Fcitx 4.2.8 REQUIRED)
find_package(Qt4 4.8 REQUIRED)
pkg_check_modules(FCITX_QT "fcitx-qt>=4.2.8")

add_subdirectory(src)

# uninstall target
#configure_file(
#  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
#  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
#  IMMEDIATE @ONLY)
# add_custom_target(uninstall
#    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

add_custom_target(debug
  COMMAND ${RUN_COMMAND} ${PROJ_KEY})