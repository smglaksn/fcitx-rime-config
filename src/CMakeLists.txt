set(FRCU_SRCS
  Main.cpp
  ConfigUtility.cpp)

set(FRCU_HDRS
  Main.h
  ConfigUtility.h)

fcitx_translate_add_sources(
  ${FRCU_SRCS}
  ${FRCU_HDRS})

include_directories(
  ${FCITX_QT_INCLUDE_DIRS}
  ${QT_QTCORE_INCLUDE_DIR}
  ${QT_QTGUI_INCLUDE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

link_directories(
  ${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
  ${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
  ${FCITX_QT_LIBRARY_DIRS})

qt4_wrap_cpp(FRCU_MOCS ${FRCU_HDRS})
qt4_wrap_ui(FRCU_SRCS FRCU.ui)

add_library(fcitx-rime-config-utility
  MODULE ${FRCU_SRCS} ${FRCU_MOCS})

set_target_properties(fcitx-rime-config-utility PROPERTIES
  LINK_FLAGS "-Wl,--no-undefined")

target_link_libraries(fcitx-rime-config-utility
  debug ${QT_QTCORE_LIBRARY_DEBUG}
  debug ${QT_QTDBUS_LIBRARY_DEBUG}
  debug ${QT_QTGUI_LIBRARY_DEBUG}
  optimized ${QT_QTCORE_LIBRARY_RELEASE}
  optimized ${QT_QTDBUS_LIBRARY_RELEASE}
  optimized ${QT_QTGUI_LIBRARY_RELEASE}
  ${FCITX4_FCITX_UTILS_LIBRARIES}
  ${FCITX4_FCITX_CONFIG_LIBRARIES}
  ${FCITX_QT_LIBRARIES}
  )

install(TARGETS fcitx-rime-config-utility DESTINATION ${FCITX4_ADDON_INSTALL_DIR}/qt)