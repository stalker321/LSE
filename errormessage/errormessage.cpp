#include <iostream>
#include "errormessage.h"

void errorLog (QString errorMessage, bool closeProgram) {
    QFile log  ("ErrorLog.txt");
    if (!log.open(QIODevice::Append)) {
        exit (1);
        std::wcout << "\033[1;31mFile opening error ErrorLog.txt!\n\033[0m";
    }
    QByteArray time (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    log.write(time + " - " + errorMessage.toStdString().c_str() + '\n');
    log.close();
    //I don't like the way it looks, I'll redo it later (maybe)
    std::wcout << "\033[1;31mNew error message!\n\033[0m";
    if (closeProgram) exit (1);
}
