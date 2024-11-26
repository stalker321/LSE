#include <fcntl.h>
#include <io.h>

#include "networkrequests.h"
#include "programmessage.h"
#include "interaction.h"

QList<QString> format {".txt", ".rft"};
QString requestsPath  ("requests.json");
QString stopWordPath  ("stopWord.json");
QString configPath      ("config.json");
QString version               ("1.0.0");

//global variable
QList<QString> stopWord;
int numberOfResponses (1);

int main(int argc, char *argv[]){
//preparation
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
//start
    QCoreApplication qa (argc, argv);
//
    QThread *message  = new QThread;
    QThread *input    = new QThread;
//
    DisplayMessage* displayMessage = new DisplayMessage;
    Interaction*    interaction    = new Interaction;
//
    displayMessage->moveToThread(message);
    interaction->moveToThread(input);
//connecting to parallel threads at startup
    QObject::connect(input,            SIGNAL(started()),              interaction, SLOT(userInput()));
    QObject::connect(message,          SIGNAL(started()),           displayMessage, SLOT(resources()));
//
    message->start();
// json
    SistemJson* info = new SistemJson;
    RequestJson* request = new RequestJson;
    BlacklistOfWords *blackList = new BlacklistOfWords;
//
    info->setConfigCheck(configPath, version);
    QList<QString> path;
    for (auto &var : info->getInfo()["files"].toArray().toVariantList()) {
        path.append(var.toString());
    }
    numberOfResponses = info->getInfo()["config"]["max_responses"].toInt();
    if (numberOfResponses < 1) numberOfResponses = 1;
//
    blackList->setBlackList(stopWordPath);
    for (auto &var : blackList->getBlacklist()["stop-word"].toArray().toVariantList()) {
        stopWord.append(var.toString());
    }
//search
    MainSearchEngine *mainSearchEngine = new MainSearchEngine(path, format);
    QString pathNext ("nextLevelPlay.txt");
    mainSearchEngine->addBase(pathNext, format, mainSearchEngine->getSearchArchive());
//json
    request->setSearchQuery(requestsPath);
//processing requests from a file
    QList<QFuture<void>> multipleSearch;
    int counterRequest = 0;
    for (auto &i : request->getRequests()["requests"].toArray().toVariantList()) {
        multipleSearch.append(QtConcurrent::run([=](){
            MainSearchEngine::dataOutput(mainSearchEngine->getHistory(), mainSearchEngine->getSearchArchive(),
                                         i.toString(), counterRequest);
        }));
        counterRequest++;
    }
    for (auto i : multipleSearch) i.waitForFinished(); //optional
//delete requests (optional)
    delete (request);
//launching network requests
//start input
    input->start();
//connect
    //blacklist
    QObject::connect(interaction,      SIGNAL(list()),              displayMessage, SLOT(displayList()));
    QObject::connect(interaction,      &Interaction::addWord, [](QString word) {
        stopWord.append(word);
    });
    QObject::connect(interaction,      &Interaction::deleteWord, [](QString word) {
        stopWord.removeOne(word);
    });
    //system messages
    QObject::connect(mainSearchEngine, SIGNAL(getMessage(QString)), displayMessage, SLOT(displayFunctionMessage(QString)));
    QObject::connect(interaction,      SIGNAL(infoMessage()),       displayMessage, SLOT(displayInfo()));
    QObject::connect(interaction,      SIGNAL(hint()),              displayMessage, SLOT(hellpInfo()));
    QObject::connect(interaction,      SIGNAL(main()),              displayMessage, SLOT(mainDisplayInfo()));
    //test request
    QObject::connect(interaction,      SIGNAL(test(QString)),     mainSearchEngine, SLOT(checkRequest(QString)));
    //exit
    QObject::connect(interaction,      SIGNAL(exit()),                         &qa, SLOT(quit()), Qt::QueuedConnection);
    QObject::connect(interaction,      &Interaction::response, [info](int max) {
        if (max == 0) info->setMaxResponsec(numberOfResponses);
        else if (max < 0) return;
        else numberOfResponses = max;
    });
    QObject::connect(interaction, &Interaction::base, qApp, [mainSearchEngine](QString path){
        mainSearchEngine->addBase(path, format, mainSearchEngine->getSearchArchive());
    });
    //delete
    QObject::connect(&qa, &QCoreApplication::aboutToQuit, qApp, [info, mainSearchEngine, displayMessage,
                                                                 interaction, blackList] () {
        delete(info);
        delete(blackList);
        delete(mainSearchEngine);
        delete(displayMessage);
        delete(interaction);
    });

    return qa.exec();
}
