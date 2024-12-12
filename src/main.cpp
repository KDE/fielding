// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QtQml>
#include <QUrl>

#include <KAboutData>
#include <KLocalizedContext>
#include <KDBusService>
#include <KLocalizedString>

#include "controller.h"
#include "fieldingconfig.h"
#include "version-fielding.h"
#include <KLocalizedQmlContext>
#include <QQuickStyle>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
#ifdef Q_OS_ANDROID
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("org.kde.breeze"));
#else
    QApplication app(argc, argv);
    // Default to org.kde.desktop style unless the user forces another style
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#endif

    KLocalizedString::setApplicationDomain("fielding");
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("fielding"));

    KAboutData aboutData(
                         // The program name used internally.
                         QStringLiteral("fielding"),
                         // A displayable program name string.
                         i18nc("@title", "Fielding"),
                         // The program version string.
                         QStringLiteral(FIELDING_VERSION_STRING),
                         // Short description of what the app does.
                         i18n("REST API testing tool"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("© 2022 - 2023 KDE Community"));
    aboutData.addAuthor(i18nc("@info:credit", "Felipe Kinoshita"), i18nc("@info:credit", "Author"), QStringLiteral("kinofhek@gmail.com"), QStringLiteral("https://fhek.gitlab.io"));
    aboutData.setBugAddress("https://bugs.kde.org/enter_bug.cgi?product=fielding");
    KAboutData::setApplicationData(aboutData);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.fielding")));

    QQmlApplicationEngine engine;

    auto config = fieldingConfig::self();

    qmlRegisterSingletonInstance("org.kde.fielding.config", 1, 0, "Config", config);

    KLocalization::setupLocalizedContext(&engine);

    engine.loadFromModule(QStringLiteral("org.kde.fielding"), QStringLiteral("Main"));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    KDBusService service(KDBusService::Unique);

    return app.exec();
}
