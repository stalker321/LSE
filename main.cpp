#include <QDebug>

#include "programmessage.h"
#include "interaction.h"

QList<QString> format {".txt", ".rft"};
QString requestsPath  ("requests.json");
QString configPath      ("config.json");
QString version               ("1.0.0");

//global variable
QList<QString> stopWord;

int main(int argc, char *argv[]){
//start
    QCoreApplication qa (argc, argv);
    QThread *message  = new QThread;
    QThread *input    = new QThread;
    DisplayMessage* displayMessage = new DisplayMessage;
    Interaction*    interaction    = new Interaction;
    displayMessage->moveToThread(message);
    interaction->moveToThread(input);
    QObject::connect(input,       SIGNAL(started()),        interaction, SLOT(userInput()));
    QObject::connect(message,     SIGNAL(started()),     displayMessage, SLOT(resources()));
    message->start();
    input->start();
// json
    SistemJson* info = new SistemJson;
    info->setConfigCheck(configPath, version);
    QList<QString> path;
    for (auto &var : info->getInfo()["files"].toArray().toVariantList()) {
        path.append(var.toString());
    }
    for (auto &var : info->getInfo()["stop-word"].toArray().toVariantList()) {
        stopWord.append(var.toString());
    }
//searh
    MainSearchEngine *mainSearchEngine = new MainSearchEngine(path, format);
    QString pathNext ("nextLevelPlay.txt");
    mainSearchEngine->addBase(pathNext, format, mainSearchEngine->getSearchArchive());
//json
    info->setSearchQuery(requestsPath);
//search
    QList<QFuture<void>> multipleSearch;
    int counterRequest = 0;
    for (auto &i : info->getInfo()["requests"].toArray().toVariantList()) {
        multipleSearch.append(QtConcurrent::run([=](){
            MainSearchEngine::dataOutput(mainSearchEngine->getHistory(), mainSearchEngine->getSearchArchive(),
                                         i.toString(), counterRequest);
        }));
        counterRequest++;
    }

    //connect
    QObject::connect(interaction, SIGNAL(infoMessage()), displayMessage, SLOT(displayInfo()));
    QObject::connect(interaction, SIGNAL(hint()),        displayMessage, SLOT(hellpInfo()));
    QObject::connect(interaction, SIGNAL(main()),        displayMessage, SLOT(mainDisplayInfo()));
    QObject::connect(interaction, &Interaction::base, qApp, [mainSearchEngine](QString path){
        mainSearchEngine->addBase(path, format, mainSearchEngine->getSearchArchive());
    });
    QObject::connect(interaction, SIGNAL(exit()),                   &qa, SLOT(quit()), Qt::QueuedConnection);
//delete
    QObject::connect(&qa, &QCoreApplication::aboutToQuit, qApp, [info, mainSearchEngine, displayMessage, interaction] () {
        delete(info);
        delete(mainSearchEngine);
        delete(displayMessage);
        delete(interaction);
    });

    for (auto i : multipleSearch) i.waitForFinished();
    return qa.exec();
}
