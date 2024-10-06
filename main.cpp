#include <iostream>
#include <QDebug>

#include "WorkingWithJson.h"
#include "startsearch.h"

QString configPath      ("config.json");
QString version               ("0.0.1");
QVector<QString> format {".txt", ".rft",
                         ".doc", "docx",
                         ".pdf" ".odt"};


int main(){
    QString path = configCheck(configPath, version);
    DocumentBase doc (path, format);
}
