#include "WebRacetimingInterface.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>

namespace RacetimingInterface {
    WebRacetimingInterface::WebRacetimingInterface(const std::string_view endpoint, const std::string_view apiKey) :
        m_endpoint(endpoint),
        m_apiKey(apiKey)
    {
        QObject::connect(&m_networkAccessManager, &QNetworkAccessManager::finished, this, &WebRacetimingInterface::networkAccessManager_finished);
    }

    RacesTable ParseRacesJson(const QJsonValue& object)
    {
        RacesTable table;
        const auto array = object.toArray();
        for (const auto& row : array) {
            const auto obj = row.toObject();
            Race race;
            race.name = obj["name"].toString().toStdString();
            race.id = obj["id"].toString().toInt();
            table.push_back(race);
        }
        return table;
    }

    RunnersTable ParseRunnersJson(const QJsonValue& object)
    {
        RunnersTable table;
        const auto array = object.toArray();
        for (const auto& row : array) {
            const auto obj = row.toObject();
            Runner runner;
            const auto hej = obj["runnerid"];
            runner.id = obj["runnerid"].toInt();
            runner.number = obj["number"].toInt();
            runner.numberid = obj["numberid"].toInt();
            const auto tags = obj["tids"].toArray();
            for (const auto& tag : tags)
                runner.tags.push_back(tag.toString().toStdString());
            runner.name = obj["name"].toString().toStdString();
            runner.surname = obj["surname"].toString().toStdString();
            table.push_back(runner);
        }
        return table;
    }

    void WebRacetimingInterface::networkAccessManager_finished(QNetworkReply* reply)
    {
        const auto replyBytes = reply->readAll();
        const auto str(replyBytes);

        qDebug() << "Get reply" << str;
        const auto document = QJsonDocument::fromJson(replyBytes);

        const auto documentObject = document.object();
        if (documentObject.contains("races")) {
            const auto races = documentObject["races"];
            if (m_racesTableUpdatedCallback)
                m_racesTableUpdatedCallback(ParseRacesJson(races));
        }

        if (documentObject.contains("runners")) {
            const auto runners = documentObject["runners"];
            if (m_runnersTableUpdatedCallback)
                m_runnersTableUpdatedCallback(ParseRunnersJson(runners));
        }
        reply->deleteLater();
    }

    void WebRacetimingInterface::requestRaces()
    {
        const auto url = m_endpoint + "/getraces_rest.php?apikey=" + m_apiKey;
        qDebug() << "Requesting " + QString::fromStdString(url);
        m_networkAccessManager.get(QNetworkRequest(QString::fromStdString(url)));
    }

    void WebRacetimingInterface::requestRunners(const int raceID)
    {
        const auto url = m_endpoint + "/getrunners_rest.php?apikey=" + m_apiKey + "&raceid=" + std::to_string(raceID);
        qDebug() << "Requesting " << QString::fromStdString(url);
        m_networkAccessManager.get(QNetworkRequest(QString::fromStdString(url)));
    }

    std::string formatTime(const struct tm& time, std::string_view format)
    {
        char dateTimeBuffer[100];
        ::strftime(dateTimeBuffer, sizeof(dateTimeBuffer) - 1, format.data(), &time);
        return std::string(dateTimeBuffer);
    }

    void WebRacetimingInterface::sendEvent(int numberID, const tm& timestamp, EventType event)
    {
        QUrlQuery postData;
        postData.addQueryItem("apikey", QString::fromStdString(m_apiKey));
        postData.addQueryItem("numberid", QString::number(numberID));
        postData.addQueryItem("timestamp", QString::fromStdString(formatTime(timestamp, "%Y-%m-%d %H:%M:%S")));
        postData.addQueryItem("msecs", "0");
        postData.addQueryItem("event", QString::number(static_cast<int>(event)));

        QNetworkRequest request(QString::fromStdString(m_endpoint) + "/addevent_rest.php");
        request.setHeader(QNetworkRequest::ContentTypeHeader,
            "application/x-www-form-urlencoded");
        m_networkAccessManager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }

    void WebRacetimingInterface::attachTag(int numberID, std::string_view tag)
    {
        QUrlQuery postData;
        postData.addQueryItem("apikey", QString::fromStdString(m_apiKey));
        postData.addQueryItem("numberid", QString::number(numberID));
        postData.addQueryItem("tid", QString::fromStdString(std::string(tag)));

        QNetworkRequest request(QString::fromStdString(m_endpoint) + "/addtag_rest.php");
        request.setHeader(QNetworkRequest::ContentTypeHeader,
            "application/x-www-form-urlencoded");
        m_networkAccessManager.post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    }
}
