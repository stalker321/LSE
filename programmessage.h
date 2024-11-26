#ifndef PROGRAMMESSAGE_H
#define PROGRAMMESSAGE_H
#include <QElapsedTimer>
#include <windows.h>
#include <QProcess>
#include <iostream>
#include <psapi.h>
#pragma comment( lib, "psapi.lib" )

#include "searchEngine.h"

class DisplayMessage : public QObject {
    Q_OBJECT
public:
    DisplayMessage();
    ~DisplayMessage();
private:
    double display = false;
    QString cpu;
    QString ram;
    QString info;
    QElapsedTimer timer;
    QByteArray timeStartProgramm;
    void programInfoMessage();
private slots:
    void displayList();
    void displayInfo();
    void hellpInfo();
    void resources();
    void mainDisplayInfo();
    void displayFunctionMessage(QString message);
};

#endif // PROGRAMMESSAGE_H
