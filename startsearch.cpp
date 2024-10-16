#include <fstream>
#include <QFile>
#include <string>
#include <QDebug>
#include "startsearch.h"
#include "errormessage.h"



QString configCheck (QString config, QString currentVersion) {
    QFile fileConfig (config);
    if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileConfig.close();
        errorLog("error in the config file", true);
    }

    json configJson (json::parse(fileConfig.readAll().toStdString()));
    if (configJson["config"]["version"] != currentVersion.toStdString()) {
        fileConfig.close();
        errorLog("mismatch of project versions", true);
    }

    fileConfig.close();
    return QString::fromStdString(configJson["files"]);
}

json search_query (QString& path) {
    QFile reqFile(path);
    if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        reqFile.close();
        errorLog("Search query file error", true);
    }
    json reqJson = json::parse(reqFile.readAll());
    reqFile.close();
    if (reqJson["requests"].size() < 1) errorLog("The search query file is empty", true);
    return reqJson;
}
