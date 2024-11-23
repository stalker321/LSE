#include <iostream>
#include "errormessage.h"

void errorLog (QString errorMessage, bool closeProgram) {
    QFile log  ("ErrorLog.txt");
    if (!log.open(QIODevice::Append)) {
        exit (1);
        std::cout << "File opening error ErrorLog.txt!\n";
    }
    QByteArray time (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    log.write(time + " - " + errorMessage.toStdString().c_str() + '\n');
    log.close();
    //переделать
    std::cout << "New error message!\n";
    if (closeProgram) exit (1);
}
