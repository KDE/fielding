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

constexpr auto APPLICATION_ID = "org.kde.fielding";

#include "about.h"
#include "version-fielding.h"
#include "fieldingconfig.h"
#include "app.h"
#include "controller.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
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
                         i18n("Simple REST API testing tool"),
                         // The license this code is released under.
                         KAboutLicense::GPL,
                         // Copyright Statement.
                         i18n("© 2022"));
    aboutData.addAuthor(i18nc("@info:credit", "Felipe Kinoshita"), i18nc("@info:credit", "Author"), QStringLiteral("kinofhek@gmail.com"), QStringLiteral("https://fhek.gitlab.io"));
    aboutData.setBugAddress("https://invent.kde.org/fhek/fielding/-/issues/new");
    KAboutData::setApplicationData(aboutData);
    QGuiApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("org.kde.fielding")));

    QQmlApplicationEngine engine;

    auto config = fieldingConfig::self();

    qmlRegisterSingletonInstance(APPLICATION_ID, 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance(APPLICATION_ID, 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance(APPLICATION_ID, 1, 0, "App", &application);

    Controller controller;
    qmlRegisterSingletonInstance(APPLICATION_ID, 1, 0, "Controller", &controller);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    KDBusService service(KDBusService::Unique);

    // Restore window size and position
    const auto rootObjects = engine.rootObjects();
    for (auto obj : rootObjects) {
        auto view = qobject_cast<QQuickWindow *>(obj);
        if (view) {
            if (view->isVisible()) {
                application.restoreWindowGeometry(view);
            }
            break;
        }
    }

    return app.exec();
}
