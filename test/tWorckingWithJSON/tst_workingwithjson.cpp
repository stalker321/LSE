#include <QtTest>

// add necessary includes here
#include "WorkingWithJson.h"
#include "qtestcase.h"

QList<QString> stopWord;
int numberOfResponses(2);

class tWorkingWithJson : public QObject
{
    Q_OBJECT

public:
    tWorkingWithJson();
    ~tWorkingWithJson();

private slots:
    void hsitory();
    void blacklist();
    void request();
    void sistemJson();
};

tWorkingWithJson::tWorkingWithJson() {}

tWorkingWithJson::~tWorkingWithJson() {}

void tWorkingWithJson::hsitory() {
    History tHistory;
    tHistory.setSearchEmpty(true, "req1");
    tHistory.setSearchEmpty(false, "req2");

    QMap<double, QVector<int>> result;
    tHistory.setRecordingResponses("req1", result);
    result[0.5].append(1);
    result[1].append(0);
    tHistory.setRecordingResponses("req2", result);
//
    QVERIFY2(tHistory.getCollectHistory()["answer"]["req1"]["result"].toBool() == false
            && tHistory.getCollectHistory()["answer"]["req2"]["result"].toBool() == true
            && tHistory.getCollectHistory()["answer"]["req2"]["relevance"][0]["docid"].toInt() == 0
            && tHistory.getCollectHistory()["answer"]["req2"]["relevance"][1]["docid"].toInt() == 1, "writing in the history");
}

void tWorkingWithJson::blacklist(){
    BlacklistOfWords tBlacklistOfWords;
    QString path ("stopWord.json");
    tBlacklistOfWords.setBlackList(path);
//
    QVERIFY2(tBlacklistOfWords.getBlacklist()["stop-word"][3].toString() == "test3", "Test blackList");
}

void tWorkingWithJson::request() {
    RequestJson tRequest;
    QString tPath("requests.json");
    tRequest.setSearchQuery(tPath);
//
    QVERIFY2(tRequest.getRequests()["requests"][0].toString() == "test", "Test request");
}

void tWorkingWithJson::sistemJson(){
    SistemJson tSistemJson;
    QString tPath("config.json");
    QString tVersion("test1.1.3");
    tSistemJson.setConfigCheck(tPath, tVersion);
    tSistemJson.setMaxResponsec(5);
    //
    QVERIFY2(tSistemJson.getInfo()["config"]["max_responses"].toInt() == 5, "Test config");
}

QTEST_APPLESS_MAIN(tWorkingWithJson)

#include "tst_workingwithjson.moc"
