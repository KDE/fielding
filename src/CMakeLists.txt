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
    KF${QT_MAJOR_VERSION}::I18n
    KF${QT_MAJOR_VERSION}::CoreAddons
    KF${QT_MAJOR_VERSION}::ConfigCore
    KF${QT_MAJOR_VERSION}::ConfigGui
    KF${QT_MAJOR_VERSION}::DBusAddons)

kconfig_add_kcfg_files(fielding GENERATE_MOC fieldingconfig.kcfgc)
install(TARGETS fielding ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
