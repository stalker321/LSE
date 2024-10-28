#include <iostream>
#include <QDebug>

#include "WorkingWithJson.h"

QVector<QString> format {".txt", ".rft"};
QString requestsPath  ("requests.json");
QString configPath      ("config.json");
QString version               ("0.1.0");

int main(){
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
//json
    JsonWork* info = new JsonWork;
    info->configCheck(configPath, version);
    QString path = QString::fromStdString(info->get_fileIdex()["files"]);
    for (auto i = info->get_fileIdex()["stop-word"].begin(); i != info->get_fileIdex()["stop-word"].end(); i++){
        stop_word.append(QString::fromStdString(i.value()));
    }
    delete(info);
    info = nullptr;
    DocumentBase* search_archive = new DocumentBase (path, format);
//json
    JsonWork* query = new JsonWork;
    query->search_query(requestsPath);
//search
    QList<QFuture<void>> multiple_search;
    int counter_request = 0;
    for (auto &i : query->get_fileIdex()["requests"]) {
        multiple_search.append(QtConcurrent::run(
        MainEngine::dataOutput, QString::fromStdString(i),
        search_archive->get_document(), counter_request));
        counter_request++;
    }
    for (auto i : multiple_search) i.waitForFinished();
    MainEngine::write_history();
    //delete
    delete (query);
    delete (search_archive);
}
