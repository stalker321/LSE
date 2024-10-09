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
    SearchEngine* search = new SearchEngine (requestsPath, path, format);

    delete (search);
}
