#include "searchengine.h"

std::mutex engineMut;

//multithreading
void MainSearchEngine::dataOutput (History* staticHistory, DocumentBase* staticSearchArchive,
                       QString req, int idRequest) {
    SearchServer *searchServer = new SearchServer;
    searchServer->createresponce(req, staticSearchArchive);
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

MainSearchEngine::~MainSearchEngine() {
    writeHistory();
    delete(history);
    delete(searchArchive);
}
