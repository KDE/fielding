// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>

#include "controller.h"

Controller::Controller(QObject* parent)
    : QObject(parent)
{
}

void Controller::fetch(QUrl url, QJsonObject options)
{
    // handle data
    QJsonDocument doc(data());
    QByteArray inputData = doc.toJson();

    // handle request and timeout
    int timeout = options.value(QStringLiteral("timeout")).toInt();
    QNetworkRequest request = QNetworkRequest(url);
    request.setTransferTimeout(timeout);

    // handle headers
    auto headers = options.value(QStringLiteral("headers")).toObject().toVariantMap();
    QMap<QString, QVariant>::const_iterator j = headers.constBegin();
    while (j != headers.constEnd()) {
        request.setRawHeader(QByteArray(j.key().toLocal8Bit()), QByteArray(j.value().toString().toLocal8Bit()));
        j++;
    }

    // handle reply
    QNetworkReply* reply;
    QString method = options.value(QStringLiteral("method")).toString();

    if (method == QStringLiteral("get")) {
        reply = m_manager.get(request);
    } else if (method == QStringLiteral("post")) {
        reply = m_manager.post(request, inputData);
    } else if (method == QStringLiteral("put")) {
        reply = m_manager.put(request, inputData);
    } else if (method == QStringLiteral("patch")) {
        reply = m_manager.sendCustomRequest(request, "PATCH", inputData);
    } else if (method == QStringLiteral("delete")) {
        reply = m_manager.deleteResource(request);
    } else {
        reply = m_manager.get(request);
    }

    // read data
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        const QString string = QString::fromUtf8(reply->readAll());
        QJsonParseError error;
        QJsonDocument data = QJsonDocument::fromJson(string.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            auto formattedString = QString::fromLatin1(data.toJson());
            Q_EMIT response(formattedString);
        } else {
            Q_EMIT response(string);
        }
        Q_EMIT status(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());

        reply->deleteLater(); // make sure to clean up
    });
}
