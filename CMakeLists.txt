# SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
# SPDX-License-Identifier: BSD-2-Clause

cmake_minimum_required(VERSION 3.16)

project(fielding VERSION 0.1)

include(FeatureSummary)

set(QT_MIN_VERSION 6.7)
set(KF_MIN_VERSION 6.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(ECMSetupVersion)
include(ECMGenerateHeaders)
include(ECMPoQmTools)

ecm_setup_version(${PROJECT_VERSION}
    VARIABLE_PREFIX FIELDING
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/src/version-fielding.h"
)

find_package(Qt6 ${QT_MIN_VERSION} REQUIRED COMPONENTS Core Gui Qml QuickControls2 Svg Widgets)
qt_policy(SET QTP0001 NEW)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED COMPONENTS Kirigami CoreAddons Config I18n DBusAddons)
find_package(KF6KirigamiAddons 1.1.0 REQUIRED)

add_subdirectory(src)

install(PROGRAMS org.kde.fielding.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.kde.fielding.metainfo.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
install(FILES org.kde.fielding.svg DESTINATION ${KDE_INSTALL_FULL_ICONDIR}/hicolor/scalable/apps)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
