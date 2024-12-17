#include <QtTest>
#include <qtestcase.h>

// add necessary includes here
#include "searchengine.h"

QList<QString> stopWord = {"stop", "stop2"};
int numberOfResponses = 2;

class tSearchEngine : public QObject
{
    Q_OBJECT

public:
    tSearchEngine();
    ~tSearchEngine();

private slots:
    void tSearch();
    void tMainSearchEngine();
};

tSearchEngine::tSearchEngine() {}

tSearchEngine::~tSearchEngine() {}

void tSearchEngine::tSearch() {
    QList<QString> format  = {".txt", ".rft"};
    QList<QString> path ("test1.txt");
    DocumentBase tBase(path, format);
    QString path2 ("test2");
    tBase.setBase(path2, format);
    QVERIFY2(tBase.getIndexBase()["test"].begin()->second == 2 && tBase.getIndexBase()["test2"][0].second == 1
            && tBase.getIndexBase()["test2"][1].second == 2 && tBase.getIndexBase()["test1"][0].second == 0, "Test base");
    SearchServer tServer;
    SearchServer tServer2;
    QString req ("test2");
    QString emptyReq("t");
    tServer.createResponce(req, &tBase);
    tServer2.createResponce(emptyReq, &tBase);
    QVERIFY2(tServer.getSearchResponse().toStdMap().begin()->second.back() == 2
             && tServer2.getSearchResponse().empty(), "Test search");
}

void tSearchEngine::tMainSearchEngine() {
    QList<QString> format  = {".txt", ".rft"};
    QList<QString> path ("test1.txt");
    MainSearchEngine tMainSearch(path, format);
    QString path2 ("test2");
    tMainSearch.addBase(path2, format, tMainSearch.getSearchArchive());
    QVERIFY2(tMainSearch.getSearchArchive()->getIndexBase()["test"].begin()->second == 2 && tMainSearch.getSearchArchive()->getIndexBase()["test2"][0].second == 1
                 && tMainSearch.getSearchArchive()->getIndexBase()["test2"][1].second == 2 && tMainSearch.getSearchArchive()->getIndexBase()["test1"][0].second == 0, "Test main base");
    QString req ("test");
    tMainSearch.beginningSearch(req, 0);
    QVERIFY2(tMainSearch.getHistory()->getCollectHistory()["answer"]["request0"]["relevance"][0]["docid"].toInt() == 2, "Test main search");
}

QTEST_APPLESS_MAIN(tSearchEngine)

#include "tst_searchengine.moc"
