#include "racetiminginterface.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

namespace RaceTiming {
    RaceTimingInterface::RaceTimingInterface(const QUrl& url, const QString& apiKey) :
        m_url("http://" + url.toString()),
        m_apiKey(apiKey)
    {
        QObject::connect(&m_networkAccessManager, &QNetworkAccessManager::finished, this, &RaceTimingInterface::networkAccessManager_finished);
    }

    void ParseJson(const QJsonValue& object, QStandardItemModel& model)
    {
        model.clear();

        const auto array = object.toArray();
        QVector<QVariantMap> data;
        for (const auto& row : array)
            data.push_back(row.toObject().toVariantMap());

        model.setRowCount(data.size());
        // Extract column data from first entry
        model.setColumnCount(data[0].size());
        const auto fieldNames = data[0].keys();
        model.setHorizontalHeaderLabels(fieldNames);

        for (int row = 0; row < data.size(); row++) {
            auto i = data[row].constBegin();
            while (i != data[row].constEnd()) {
                auto col = fieldNames.indexOf(i.key());
                model.setItem(row, col, new QStandardItem(i.value().toString()));
                ++i;
            }
        }
    }

    void RaceTimingInterface::networkAccessManager_finished(QNetworkReply* reply)
    {
        const auto replyBytes = reply->readAll();
        const auto str(replyBytes);

        qDebug() << "Get reply" << str;
        const auto document = QJsonDocument::fromJson(replyBytes);

        const auto documentObject = document.object();
        if (documentObject.contains("races")) {
            const auto races = documentObject["races"];
            ParseJson(races, m_racesTable);
            emit gotRaces();
        }

        if (documentObject.contains("runners")) {
            const auto runners = documentObject["runners"];
            ParseJson(runners, m_runnersTable);
            emit gotRunners();
        }
        reply->deleteLater();
    }

    void RaceTimingInterface::getRaces()
    {
        QUrlQuery query;
        query.addQueryItem("apikey", m_apiKey);
        const auto url = m_url.toString() + "/getraces_rest.php?" + query.toString();
        qDebug() << "Requesting " + url;
        m_networkAccessManager.get(QNetworkRequest(url));
    }

    void RaceTimingInterface::getRunners(const int raceID)
    {
        QUrlQuery query;
        query.addQueryItem("apikey", m_apiKey);
        query.addQueryItem("raceid", QString::number(raceID));

        const auto url = m_url.toString() + "/getrunners_rest.php?" + query.toString();
        qDebug() << "Requesting " + url;
        m_networkAccessManager.get(QNetworkRequest(url));
    }

    void RaceTimingInterface::sendEvent(int numberID, QDateTime timestamp, EventType event)
    {
        QUrlQuery postData;
        postData.addQueryItem("apikey", m_apiKey);
        postData.addQueryItem("numberid", QString::number(numberID));
        postData.addQueryItem("timestamp", timestamp.toString("yyyy-MM-dd hh:mm:ss"));
        postData.addQueryItem("msecs", timestamp.toString("zzz"));
        postData.addQueryItem("event", QString::number(static_cast<int>(event)));

        QNetworkRequest request(m_url.toString() + "/addevent_rest.php");
        request.setHeader(QNetworkRequest::ContentTypeHeader,
            "application/x-www-form-urlencoded");
        m_networkAccessManager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }
}
