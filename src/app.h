// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>
#include <QQmlEngine>
#include <QQuickWindow>

class App : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit App(QObject* parent = nullptr);

    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow* window);
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow* window);

private:
};
