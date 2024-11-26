#include "WorkingWithJson.h"

void SistemJson::setConfigCheck (QString& config, QString& currentVersion) {
    path = config;
    QFile fileConfig (path);
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


void SistemJson::setMaxResponsec (int max){
    QJsonObject root = info.object();
    QJsonObject config = root["config"].toObject();
    config["max_responses"] = max;
    root["config"] = config;
    info.setObject(root);
}

SistemJson::~SistemJson() {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        file.close();
        errorLog("error in the config file", false);
        return;
    }

    file.write(info.toJson(QJsonDocument::Indented));
    file.close();
}
