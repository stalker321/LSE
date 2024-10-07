#include <iostream>
#include <QDebug>

#include "WorkingWithJson.h"
#include "startsearch.h"

QString configPath      ("config.json");
QString version               ("0.0.1");
QVector<QString> format {".txt", ".rft"};


int main(){
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QString path = configCheck(configPath, version);
    DocumentBase doc (path, format);
}
