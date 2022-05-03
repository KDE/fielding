// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "version-fielding.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "fieldingconfig.h"

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

    qmlRegisterSingletonInstance("org.kde.fielding", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.fielding", 1, 0, "AboutType", &about);

    Controller controller;
    qmlRegisterSingletonInstance("org.kde.fielding", 1, 0, "Controller", &controller);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
