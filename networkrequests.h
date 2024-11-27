#ifndef NETWORKREQUESTS_H
#define NETWORKREQUESTS_H
#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

#include "searchEngine.h"

class NetworkRequests : public QTcpServer {
    Q_OBJECT
public:
    NetworkRequests(MainSearchEngine *Engine);
protected:
    void incomingConnection(qintptr socketDescriptor) override;
    MainSearchEngine *engine = nullptr;
};

#endif // NETWORKREQUESTS_H
