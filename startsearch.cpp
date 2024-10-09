#include <fstream>
#include <QFile>
#include <string>
#include <QDebug>
#include "startsearch.h"
#include "errormessage.h"

#include "nlohmann/json.hpp"

using json = nlohmann::json;

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
