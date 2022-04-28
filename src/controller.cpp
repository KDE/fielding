// SPDX-FileCopyrightText: 2022 Felipe Kinoshita <kinofhek@gmail.com>
// SPDX-License-Identifier: LGPL-2.1-or-later

#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>


#include "controller.h"

void Controller::fetch(QUrl url, QJsonObject options)
{
    // handle params
    QJsonObject params = options.value("params").toObject();
    QUrlQuery query;

    auto map = params.toVariantMap();
    QMap<QString, QVariant>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        query.addQueryItem(i.key(), i.value().toString());
        i++;
    }

    url.setQuery(query.query());

    // handle data
    QJsonDocument doc(data());
    QByteArray inputData = doc.toJson();

    // handle request and timeout
    int timeout = options.value("timeout").toInt();
    QNetworkRequest request = QNetworkRequest(url);
    request.setTransferTimeout(timeout);

    // handle headers
    auto headers = options.value("headers").toObject().toVariantMap();
    QMap<QString, QVariant>::const_iterator j = headers.constBegin();
    while (j != headers.constEnd()) {
        request.setRawHeader(QByteArray(j.key().toLocal8Bit()), QByteArray(j.value().toString().toLocal8Bit()));
        j++;
    }

    // handle reply
    QNetworkReply* reply;
    QString method = options.value("method").toString();

    if (method == "get") {
        reply = m_manager.get(request);
    } else if (method == "post") {
        reply = m_manager.post(request, inputData);
    } else if (method == "put") {
        reply = m_manager.put(request, inputData);
    } else if (method == "patch") {
        reply = m_manager.sendCustomRequest(request, "PATCH", inputData);
    } else if (method == "delete") {
        reply = m_manager.deleteResource(request);
    } else {
        reply = m_manager.get(request);
    }

    // read data
    QObject::connect(reply, &QNetworkReply::finished, [this, reply]() {
        QString replyText = reply->readAll();
        QJsonObject data = QJsonDocument::fromJson(replyText.toUtf8()).object();

        // response object
        QJsonObject obj = QJsonObject({
            {"response", data},
            {"status", reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()},
            {"statusText", reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()}
        });

        QJsonDocument doc(obj);
        QByteArray ba = doc.toJson();
        QString string = QString(ba);

        Q_EMIT response(string);

        reply->deleteLater(); // make sure to clean up
    });
}
