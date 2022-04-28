# SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(roy
    main.cpp
    about.cpp
    controller.cpp
    resources.qrc)

target_link_libraries(roy
    Qt5::Core
    Qt5::Gui
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    Qt5::Svg
    KF5::I18n
    KF5::CoreAddons
    KF5::ConfigCore
    KF5::ConfigGui)

kconfig_add_kcfg_files(roy GENERATE_MOC royconfig.kcfgc)
install(TARGETS roy ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
