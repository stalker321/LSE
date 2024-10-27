#include <iostream>
#include <QDebug>

#include "WorkingWithJson.h"
#include "startsearch.h"

QVector<QString> format {".txt", ".rft"};
QString requestsPath  ("requests.json");
QString configPath      ("config.json");
QString version               ("0.0.3");

int main(){
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    json info = configCheck(configPath, version);
    QString path = QString::fromStdString(info["files"]);
    for (auto i = info["stop-word"].begin(); i != info["stop-word"].end(); i++){
        stop_word.append(QString::fromStdString(i.value()));
    }
    DocumentBase* search_archive = new DocumentBase (path, format);
    json query (search_query(requestsPath));

    //search
    QList<QFuture<void>> multiple_search;
    int counter_request = 0;
    for (auto &i : query["requests"]) {
        multiple_search.append(QtConcurrent::run(
                MainEngine::dataOutput, QString::fromStdString(i),
                search_archive->get_fileIndex(), counter_request));
        counter_request++;
    }
    for (auto i : multiple_search) i.waitForFinished();
    MainEngine::write_history();
    //delete
    delete (search_archive);
}
