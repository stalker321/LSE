#include "WorkingWithJson.h"

void RequestJson::setSearchQuery (QString& path) {
    QFile reqFile(path);
    if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        reqFile.close();
        //error message
        errorLog("Search query file error", true);
    }
    requests = QJsonDocument::fromJson(reqFile.readAll());
    reqFile.close();
    if (requests["requests"].Null) errorLog("The search query file is empty", false);
}
