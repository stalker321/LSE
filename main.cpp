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
    QString path = configCheck(configPath, version);
    DocumentBase* search_archive = new DocumentBase (path, format);
    json query (search_query(requestsPath));
    //test
//    QList<QFuture<void>> multiple_search;

//    for (int i = 0; i < query["requests"].size(); i++) {
//        multiple_search.append(QtConcurrent::run(
//                MainEngine::data_output, QString::fromStdString(query["requests"][i]),
//                search_archive->get_fileIndex()));
//    }

//    for (auto i : multiple_search) i.waitForFinished();

    //delete
    delete (search_archive);
}
