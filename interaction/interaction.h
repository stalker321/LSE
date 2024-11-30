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
//standard information
    void hint();
    void main();
    void infoMessage();
//
    void response(int);
    void test(QString);
    void base(QString);
//blacklist
    void list();
//Signals for editing the blacklist
    void addWord(QString);
    void deleteWord(QString);
};

#endif // INTERACTION_H
