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

void Controller::fetch(const QString &urlStr, QJsonObject options)
{
    // handle data
    QJsonDocument doc(data());
    QByteArray inputData = doc.toJson();

    QUrl url = QUrl::fromUserInput(urlStr);

    // handle request and timeout
    int timeout = options.value(QStringLiteral("timeout")).toInt();
    QNetworkRequest request = QNetworkRequest(url);
    request.setTransferTimeout(timeout);

    // handle headers
    auto headers = options.value(QStringLiteral("headers")).toObject().toVariantMap();
    for (auto [key, value] : headers.asKeyValueRange()){
        request.setRawHeader(key.toUtf8(), value.toString().toUtf8());
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
        const auto contentType = reply->header(QNetworkRequest::KnownHeaders::ContentTypeHeader).toString();

        const QString string = QString::fromUtf8(reply->readAll());
        QJsonParseError error;
        QJsonDocument data = QJsonDocument::fromJson(string.toUtf8(), &error);
        if (error.error == QJsonParseError::NoError) {
            auto formattedString = QString::fromLatin1(data.toJson());
            Q_EMIT response(formattedString, QStringLiteral("JSON"));
        } else {
            Q_EMIT response(string, contentTypeToDefinition(contentType));
        }
        Q_EMIT status(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString());

        reply->deleteLater(); // make sure to clean up
    });
}

QString Controller::contentTypeToDefinition(const QString &contentType)
{
    // TODO: write a unit test
    if (contentType.isEmpty()) {
        return {};
    }

    const QString mediaType = contentType.section(QLatin1Char(';'), 0, 0).trimmed().toLower();

    if (mediaType == QLatin1String("application/json")) {
        return QStringLiteral("JSON");
    } else if (mediaType == QLatin1String("text/html")) {
        return QStringLiteral("HTML");
    } else if (mediaType == QLatin1String("text/xml") || mediaType == QLatin1String("application/xml")) {
        return QStringLiteral("XML");
    } else if (mediaType == QLatin1String("application/javascript") || mediaType == QLatin1String("text/javascript")) {
        return QStringLiteral("JavaScript");
    } else if (mediaType == QLatin1String("text/css")) {
        return QStringLiteral("CSS");
    } else if (mediaType == QLatin1String("text/plain")) {
        return {};
    } else {
        qWarning() << "Unknown content type:" << mediaType;
        return {};
    }
}
