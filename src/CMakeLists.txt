set(FRCU_SRCS
  Main.cpp
  ConfigMain.cpp
  Model.cpp
  rime-utils/FcitxRimeConfig.c
  fcitx-utils/cutf8.cpp)

set(FRCU_HDRS
  Main.h
  ConfigMain.h
  Model.h
  rime-utils/FcitxRimeConfig.h)

fcitx_translate_add_sources(
  ${FRCU_SRCS}
  ${FRCU_HDRS})
  
include_directories(
  ${Qt5Widgets_INCLUDE_DIRS}
  ${Qt5Concurrent_INCLUDE_DIRS}
  ${RIME_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR})

link_directories(
  ${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
  ${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
  ${FCITX_QT_LIBRARY_DIRS}
  ${RIME_LIBRARY_DIRS})

# qt4_wrap_cpp(FRCU_MOCS ${FRCU_HDRS})
# qt4_wrap_ui(FRCU_SRCS ConfigMain.ui)
qt5_wrap_cpp(FRCU_MOCS ${FRCU_HDRS})
qt5_wrap_ui(FRCU_SRCS ConfigMain.ui)

add_library(fcitx-rime-config-gui
  MODULE ${FRCU_SRCS} ${FRCU_MOCS})

set_target_properties(fcitx-rime-config-gui PROPERTIES
  LINK_FLAGS "-Wl,--no-undefined")

target_link_libraries(fcitx-rime-config-gui
  Qt5::Widgets
  FcitxQt5::WidgetsAddons
  ${FCITX4_FCITX_UTILS_LIBRARIES}
  ${FCITX4_FCITX_UTILS_LIBRARIES}
  ${FCITX4_FCITX_CONFIG_LIBRARIES}
  ${FCITX_QT_LIBRARIES}
  ${RIME_LIBRARIES}
  )

install(TARGETS fcitx-rime-config-gui DESTINATION ${FCITX4_ADDON_INSTALL_DIR}/qt)
