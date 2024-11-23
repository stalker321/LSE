#include "WorkingWithJson.h"

void SistemJson::setConfigCheck (QString config, QString currentVersion) {
    QFile fileConfig (config);
    if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileConfig.close();
        errorLog("error in the config file", true);
    }
    info = QJsonDocument::fromJson(fileConfig.readAll());
    if (info["config"]["version"] != currentVersion) {
        fileConfig.close();
//error message
        errorLog("mismatch of project versions", true);
    }

    fileConfig.close();
}

void SistemJson::setSearchQuery (QString& path) {
    QFile reqFile(path);
    if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        reqFile.close();
//error message
        errorLog("Search query file error", true);
    }
    info = QJsonDocument::fromJson(reqFile.readAll());
    reqFile.close();
    if (info["requests"].Null) errorLog("The search query file is empty", true);
}
