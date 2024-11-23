#ifndef INTERACTION_H
#define INTERACTION_H
#include <qmutex.h>
#include <qobject.h>
#include <iostream>

class Interaction : public QObject{
    Q_OBJECT
public:
    Interaction();
private slots:
    void userInput();
signals:
    void exit();
    void hint();
    void main();
    void infoMessage();
    void base(QString path);
};

#endif // INTERACTION_H
