#include "networkrequests.h"
#include "errormessage.h"
#include <networkrequests.h>

#include <QThreadPool>
#include <qurlquery>
#include <qthread.h>
#include <QRunnable>
#include <iostream>
#include <QThread>

//start the server
NetworkRequests::NetworkRequests(MainSearchEngine *mainSearchEngine) : engine (mainSearchEngine) {
    if (!listen(QHostAddress::AnyIPv4, 1820)) {
        errorLog("failed to create a server", true);
        return;
    }
    std::wcout << "The server is running, the server port is: 1820\n";
}
//connecting clients, processing requests
void NetworkRequests::incomingConnection(qintptr socketRequest) {
    QTcpSocket *socket = new QTcpSocket();
    if (socket->setSocketDescriptor(socketRequest)) {
        std::wcout << "Client connected \n";

        //Reading the data
        connect(socket, &QTcpSocket::readyRead, [socket, this]() {
            QByteArray requestData = socket->readAll();
            QString message = QString::fromUtf8(requestData);
            // std::wcout << L"Received from client:" << message.toStdWString() << std::endl;

            QtConcurrent::run([=](){
                MainSearchEngine::dataOutput(engine->getHistory(), engine->getSearchArchive(),
                                  message, engine->getNumRequest());
            });
            engine->getNumRequest()++;

            //response to the client
            QString response = "Hello from server!";
            socket->write(response.toUtf8());
            socket->flush();
        });

        //Closing the connection
        connect(socket, &QTcpSocket::disconnected, [socket]() {
            std::wcout << "Client disconnected:" << socket->peerAddress().toString().toStdWString() << std::endl;
            socket->deleteLater();
        });
    } else {
        delete socket;
    }
}

// NetworkRequests::~NetworkRequests(){}
