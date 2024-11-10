#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QMap>
#include <QFile>
#include <QList>
#include <QHash>
#include <math.h>
#include <QDebug>
#include <QFuture>
#include <QVector>
#include <QMultiMap>
#include <QByteArray>
#include <QtConcurrent>
#include "errormessage.h"

#include "nlohmann/json.hpp"

QVector<QString> stopWord;

using json = nlohmann::json;

//working with json config/query files
struct SistemJson {
//config
    void configCheck (QString config, QString currentVersion) {
        QFile fileConfig (config);
        if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
            fileConfig.close();
            errorLog("error in the config file", true);
        }
        info = json::parse(fileConfig.readAll().toStdString());
        if (info["config"]["version"] != currentVersion.toStdString()) {
            fileConfig.close();
            errorLog("mismatch of project versions", true);
        }

        fileConfig.close();
    }
//query
    void searchQuery (QString& path) {
        QFile reqFile(path);
        if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            reqFile.close();
            errorLog("Search query file error", true);
        }
        info = json::parse(reqFile.readAll());
        reqFile.close();
        if (info["requests"].size() < 1) errorLog("The search query file is empty", true);
    }
//get
        const json &getInfo() {
            return info;
        }
protected:
    json info;
};
//response history
class History {
public:
    void setSearchEmpty (bool empty, std::string request) {
        if (empty) collectHistory["answer"][request]["result"] = false;
        else collectHistory["answer"][request]["result"] = true;
    }
    void setRecordingResponses (std::string request,
        std::unordered_map<std::string, double> rec) {
        collectHistory["answer"][request]["relevance"].push_back(rec);
    }
//get
    const json &getCollectHistory() {
        return collectHistory;
    }

private:
    json collectHistory;
};

#endif // WORKINGWITHJSON_H
