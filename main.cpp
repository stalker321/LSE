#include <iostream>
#include <QDebug>

#include "WorkingWithJson.h"
#include "startsearch.h"

QString configPath ("config.json");
QString version          ("0.0.1");

int main(){
    QString path = configCheck(configPath, version);
    DocumentBase doc (path, ".txt");
}
