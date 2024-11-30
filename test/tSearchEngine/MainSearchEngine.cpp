#include "searchengine.h"

std::mutex engineMut;

//multithreading
void MainSearchEngine::dataOutput (History* staticHistory, DocumentBase* staticSearchArchive,
                       QString req, int idRequest) {
    SearchServer *searchServer = new SearchServer;
    searchServer->createResponce(req, staticSearchArchive);
//json
    auto &searchResult = searchServer->getSearchResponse();
    QString numRequest ("request");
    numRequest += QString("%1").arg(idRequest);
    engineMut.lock();
    staticHistory->setSearchEmpty(searchResult.empty(), numRequest);
    staticHistory->setRecordingResponses(numRequest, searchResult);
    engineMut.unlock();
    delete(searchServer);
}

void MainSearchEngine::addBase(QString path, QList<QString>& format, DocumentBase* searchArchive) {
    searchArchive->setBase(path, format);
}

//search engine
MainSearchEngine::MainSearchEngine (QList<QString>& path, QList<QString>& format)  {
    searchArchive = new DocumentBase(path, format);
}

void MainSearchEngine::writeHistory () {
    history->write();
}

void MainSearchEngine::checkRequest(QString req) {
    QElapsedTimer timer;
    timer.start();
    SearchServer *searchServer = new SearchServer;
    searchServer->createResponce(req, searchArchive);
    auto time = timer.nsecsElapsed()/1000;

    QString answer;
    answer.append(QString("Request processing time: %1 us\n").arg(time));
    auto response = searchServer->getSearchResponse();
    for (auto i = response.begin(); i != response.end(); i++) {
        answer.append(QString("id - %1 relevance - %2\n")
                          .arg(i.value(), 4)
                          .arg(i.key(), 5, 'f', 4));
    }
    delete (searchServer);
    emit getMessage(answer);
}

MainSearchEngine::~MainSearchEngine() {
    writeHistory();
    delete(history);
    delete(searchArchive);
}
