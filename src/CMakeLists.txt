# SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

add_executable(fielding
    main.cpp
    controller.cpp
)

qt_add_qml_module(fielding URI org.kde.fielding
    QML_FILES
        ui/GlobalMenu.qml
        ui/Main.qml
        ui/PageHeader.qml
)

target_link_libraries(fielding PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
    Qt::QuickControls2
    Qt::Svg
    Qt::Widgets
    KF6::I18n
    KF6::I18nQml
    KF6::CoreAddons
    KF6::ConfigCore
    KF6::ConfigGui
    KF6::DBusAddons)

kconfig_target_kcfg_file(fielding FILE fieldingconfig.kcfg CLASS_NAME Config MUTATORS GENERATE_PROPERTIES GENERATE_MOC DEFAULT_VALUE_GETTERS PARENT_IN_CONSTRUCTOR QML_REGISTRATION SINGLETON)
install(TARGETS fielding ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
