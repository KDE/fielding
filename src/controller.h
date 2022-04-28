// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#pragma once

#include <QObject>
#include <QJsonObject>
#include <QNetworkAccessManager>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QJsonObject data MEMBER m_data NOTIFY dataChanged)

public:
    Q_INVOKABLE void fetch(QUrl url, QJsonObject options);

    QJsonObject data()
    {
        return m_data;
    }
    Q_INVOKABLE void setData(QJsonObject data)
    {
        m_data = data;
        Q_EMIT dataChanged();
    }
    Q_SIGNAL void dataChanged();

    Q_SIGNAL QString response(QString);

private:
    QNetworkAccessManager m_manager;
    QJsonObject m_data;
};
