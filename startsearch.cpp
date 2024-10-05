#include <fstream>
#include <QFile>
#include <QDateTime>
#include <string>
#include <QDebug>
#include "startsearch.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

void errorLog (QString errorMessage) {
    QFile log  ("ErrorLog.txt");
    if (!log.open(QIODevice::WriteOnly | QIODevice::Append)) exit (1);
    QByteArray time (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    log.write(time + " - " + errorMessage.toStdString().c_str() + '\n');
    log.close();
    exit (1);
}

QString configCheck (QString config, QString currentVersion) {
    QFile fileConfig (config);
    if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fileConfig.close();
        errorLog("error in the config file");
    }

    json configJson (json::parse(fileConfig.readAll().toStdString()));
    if (configJson["config"]["version"] != currentVersion.toStdString()) {
        fileConfig.close();
        errorLog("mismatch of project versions");
    }

    fileConfig.close();
    return QString::fromStdString(configJson["files"]);
}
