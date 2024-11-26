#include "networkrequests.h"
#include "errormessage.h"

#include <QThreadPool>
#include <qurlquery>
#include <qthread.h>
#include <QRunnable>
#include <iostream>
#include <QThread>

// NetworkRequests::NetworkRequests() {
//     if (!listen(QHostAddress::Any, 1820)) {
//         errorLog("failed to create a server", true);
//         return;
//     }
//     std::wcout << "The server is running, the server port is: 1820";
// }

// void NetworkRequests::incomingConnection(qintptr socketRequest) {
//     QThreadPool::globalInstance()->start([this, socketRequest]() {
//         QTcpSocket socket;
//         if (!socket.setSocketDescriptor(socketRequest)) {
//             errorLog("Socket processing error");
//             return;
//         }

//         QByteArray receivedData;

//         // Ожидаем пока данные не будут готовы для чтения
//         connect(&socket, &QTcpSocket::readyRead, [&]() {
//             // Читаем все доступные данные
//             receivedData.append(socket.readAll());

//             // Проверяем, пришло ли полное сообщение
//             if (receivedData.contains("\r\n")) {  // Например, мы ожидаем символ новой строки
//                 QString message = QString::fromUtf8(receivedData);
//                 std::wcout << "Message: " << message.toStdWString() << std::endl;

//                 // Отправляем ответ
//                 socket.write("Answer!");
//                 socket.flush();

//                 // Закрываем соединение
//                 socket.waitForDisconnected();
//             }
//         });

//         // Ожидаем завершения соединения
//         socket.waitForDisconnected();
//     });
// }

// NetworkRequests::~NetworkRequests(){}
