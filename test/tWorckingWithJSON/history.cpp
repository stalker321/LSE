#include "WorkingWithJson.h"

void History::setSearchEmpty(bool empty, QString request) {
    QJsonObject result;

    result.insert("result", empty ? QJsonValue(false) : QJsonValue(true));

    if (!collectHistory.contains("answer")) {
        collectHistory.insert("answer", QJsonObject());
    }

    QJsonObject answerObject = collectHistory["answer"].toObject();
    answerObject.insert(request, result);
    collectHistory["answer"] = answerObject;
}

void History::setRecordingResponses(QString request, QMap<double, QVector<int>> result) {
    if (!collectHistory.contains("answer")) {
        collectHistory.insert("answer", QJsonObject());
    }

    QJsonObject answerObject = collectHistory["answer"].toObject();
    if (!answerObject.contains(request)) {
        answerObject.insert(request, QJsonObject());
    }

    QJsonObject requestObject = answerObject[request].toObject();
    if (!requestObject.contains("relevance")) {
        requestObject.insert("relevance", QJsonObject());
    }

    QJsonArray relevanceMap = requestObject["relevance"].toArray();
    int counter = 0;

    for (auto a = result.end(); a != result.begin();) {
        --a;
        for (auto r : *a) {
            if (counter < numberOfResponses) counter++;
            else break;
            QJsonObject docRankObject;
            docRankObject["docid"] = r;
            docRankObject["rank"] = (double)floorf(a.key() * 1000) / 1000;;
            relevanceMap.append(docRankObject);
        }
        if (!(counter < numberOfResponses)) break;
    }
    requestObject["relevance"] = relevanceMap;
    answerObject[request] = requestObject;
    collectHistory["answer"] = answerObject;
}

QJsonDocument History::getAnswer (QString req) {
    QJsonObject object = collectHistory["answer"][req].toObject();
    return QJsonDocument (object);
}

void History::write() {
    QFile responseRequest("answer.json");
    if (!responseRequest.open(QIODevice::WriteOnly)) {
//error message
        QString errorMessage ("Error working with the answer file");
        errorLog(errorMessage);
        return;
    }
    responseRequest.write(QJsonDocument(collectHistory).toJson(QJsonDocument::Indented));
    responseRequest.close();
}
