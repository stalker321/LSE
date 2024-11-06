#include <iostream>
#include <QDebug>

// #if QT_VERSION_MAJOR < 6
//     #include <QTextCodec>
// #else
//     #include <QtCore5Compat/QTextCodec>
// #endif

#include "searchengine.h"

QVector<QString> format {".txt", ".rft"};
QString requestsPath  ("requests.json");
QString configPath      ("config.json");
QString version               ("0.1.0");

int main(){
// json
    SistemJson* info = new SistemJson;
    info->configCheck(configPath, version);
    QString path = QString::fromStdString(info->getInfo()["files"]);
    for (auto i = info->getInfo()["stop-word"].begin(); i != info->getInfo()["stop-word"].end(); i++){
        stopWord.append(QString::fromStdString(i.value()));
    }
//searh
    MainSearchEngine *mainSearchEngine = new MainSearchEngine(path, format);
//json
    info->searchQuery(requestsPath);
    qDebug() << to_string (info->getInfo()).c_str();
//search
    QList<QFuture<void>> multipleSearch;
    int counterRequest = 0;
    for (auto &i : info->getInfo()["requests"]) {
        multipleSearch.append(QtConcurrent::run([=](){
            mainSearchEngine->dataOutput(mainSearchEngine->getHistory(), mainSearchEngine->getSearchArchive(),
                                         QString::fromStdString(i), counterRequest);
        }));
        counterRequest++;
    }
    for (auto i : multipleSearch) i.waitForFinished();
    mainSearchEngine->writeHistory();
// // delete
    delete (mainSearchEngine);
    delete (info);
}
