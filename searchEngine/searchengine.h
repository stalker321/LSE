#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <Thread>
#include <iostream>
#include <filesystem>
#include <QtConcurrent>

#include "wordIndexing.h"
#include "WorkingWithJson.h"

namespace fs = std::filesystem;
//global variable
extern QList<QString> stopWord;
extern int numberOfResponses;

struct FileInfo {
    int id;
    QString address;
    QHash<QString, int> textIndex;
};

//database in json format
class DocumentBase {
public:
    ~DocumentBase();
    DocumentBase (QList<QString>& path, QList<QString>& format);
//add docBase
    void setBase (QString& path, QList<QString>& format);
//get
    const QMap<QString, QVector<QPair<int, int>>> getIndexBase () {
        return indexBase;
    }
protected:
    QMap<QString, QVector<QPair<int, int>>> indexBase;
    QVector<FileInfo> docBase;
    int id = 0;
};

struct SearchServer {
    void createResponce (QString& req, DocumentBase* searchArchive);
//get
    const QMultiMap<double,int> getSearchResponse(){
        return searchResponse;
    }
protected:
    QMultiMap<double,int> searchResponse;
};

class MainSearchEngine: public QObject {
    Q_OBJECT
public:
//delete
    ~MainSearchEngine();
    MainSearchEngine (QList<QString>& path, QList<QString>& format);
//write history (recording using QJson)
    void writeHistory();
//multithreading
    static void dataOutput (History* staticHistory, DocumentBase* staticSearchArchive,
                           QString req, int idRequest);
    static void addBase (QString path, QList<QString>& format, DocumentBase* searchArchive);
//get
    DocumentBase* getSearchArchive () {
        return searchArchive;
    }
    History* getHistory() {
        return history;
    }
    int &getNumRequest(){
        return numRequest;
    }
protected:
    int numRequest = 0;
    History* history = new History;
    DocumentBase* searchArchive = nullptr;
private slots:
    void checkRequest(QString req);
signals:
    void getMessage(QString message);
};

#endif // SEARCHENGINE_H
