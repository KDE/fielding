# SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(fielding
    main.cpp
    about.cpp
    app.cpp
    controller.cpp
    resources.qrc)

target_link_libraries(fielding
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::Svg
    Qt::Widgets
    KF6::I18n
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::DBusAddons)

kconfig_add_kcfg_files(fielding GENERATE_MOC fieldingconfig.kcfgc)
install(TARGETS fielding ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
