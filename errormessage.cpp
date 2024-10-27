#include "errormessage.h"

void errorLog (QString errorMessage, bool closeProgram) {
    QFile log  ("ErrorLog.txt");
    if (!log.open(QIODevice::Append)) exit (1);
    QByteArray time (QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss").toUtf8());
    log.write(time + " - " + errorMessage.toStdString().c_str() + '\n');
    log.close();
    if (closeProgram) exit (1);
}
