#include "WorkingWithJson.h"

void BlacklistOfWords::setBlackList(QString& path){
    filePath = path;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.close();
        //error message
        errorLog("Error in the blacklist file", false);
    }
    blacklist = QJsonDocument::fromJson(file.readAll());
    file.close();
}

BlacklistOfWords::~BlacklistOfWords(){
    QJsonArray jsonArray;
    for (const auto& w : stopWord) {
        jsonArray.append(w);
    }

    QJsonObject stopWords;
    stopWords["stop-word"] = jsonArray;
    blacklist.setObject(stopWords);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
        //error message
        errorLog("Error in the blacklist file", false);
    }

    file.write(blacklist.toJson(QJsonDocument::Compact));
    file.close();
}
