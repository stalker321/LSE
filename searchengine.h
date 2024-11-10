#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include <filesystem>
#include <Thread>
#include "wordIndexing.h"
#include "WorkingWithJson.h"

std::mutex mut;
namespace fs = std::filesystem;

struct FileInfo {
    int id;
    QString address;
    QHash<QString, int> textIndex;
};

//database in json format
class DocumentBase {
public:
    ~DocumentBase() {}
    DocumentBase (QString& path, QVector<QString>& format) {
        if (!fs::exists(path.toStdString())) {
            errorLog("Incorrect file path", true);
            return;
        }
        fs::path directory (path.toStdString());
        QVector<QString> paths = searchExtension(directory, format);
        if (paths.empty()) {
            errorLog("Incorrect file path", true);
            return;
        }
        searchFile(paths);
        QList<std::thread*> readIndexBase;
        for (auto &i : docBase) {
            readIndexBase.append(new std::thread(writeIndex, std::move(i), &indexBase));
        }
        for (auto *i : readIndexBase) i->join();
    }
    void searchFile (QVector<QString>& filePaths) {
        QList<QFuture<FileInfo>> informationResource;
        for (auto &p : filePaths) {
            informationResource.append(QtConcurrent::run(docIndexing, p, id));
            id++;
        }
        for (auto &w : informationResource) w.waitForFinished();
        for (auto &i : informationResource) {
            docBase.append(i.result());
        }
    }
    //multithreading
    static FileInfo docIndexing (QString path, int id) {
        FileInfo indexing;
        indexing.address = path;
        indexing.id = id;
        QFile resursec (path);

        if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
            resursec.close();
            errorLog("File reading error " + path, true);
        }

        QString text = resursec.readAll();
        resursec.close();
        indexing.textIndex = WordIndexing::indexingWord(text, stopWord);
        return indexing;
    }
    static void writeIndex (FileInfo info, QMap<QString, QVector<QPair<int, int>>> *base) {
        if (!info.textIndex.empty()){
            for (auto i = info.textIndex.begin(); i != info.textIndex.end(); i++) {
                mut.lock();
                (*base)[i.key()].append(QPair<int, int>(i.value(), info.id));
                mut.unlock();
            }
        }
    }
//add docBase
    // const void setBase (QString& path, QVector<QString> format) {
    //     if (!fs::exists(path.toStdString())) {
    //         errorLog("Incorrect add file path", false);
    //         return;
    //     }
    //     if (!fs::is_directory(path.toStdString())) {

    //     }
    // }
//collect files using the specified path
    QVector<QString> searchExtension (fs::path& dir, QVector<QString>& ext) {
        QVector<QString> paths;
        for (const fs::directory_entry& p : fs::directory_iterator(dir)){
            if (!fs::is_regular_file(p.status())) continue;
            filePath(p, ext, paths);
        }
        return paths;
    }

    void filePath (fs::directory_entry p, QVector<QString>& ext, QVector<QString>& path){
        for (auto& e : ext) {
            if (!p.path().extension().compare(e.toStdString())){
                path.append(QString::fromStdString(p.path().u8string()));
                break;
            }
        }
    }
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
    void createresponce (QString& req, DocumentBase* searchArchive) {
        auto temp = WordIndexing::indexingWord(req, stopWord);
//a blank to improve relevance
        QMultiMap<int, QString> reqIndex;
        for (auto i = temp.begin(), end = temp.end(); i != end; i++){
            reqIndex.insert(i.value(), i.key());
        }
        if (reqIndex.size() < 1) return;
//search
        QMap <int, int> intermediateSearch;
        int max = 0;
        for (auto r = reqIndex.begin(); r != reqIndex.end(); r++) {
            if (!searchArchive->getIndexBase().contains(r.value())) continue;
            for (auto &i : searchArchive->getIndexBase()[r.value()]) {
                intermediateSearch[i.second] += i.first;
                max = std::max(max,intermediateSearch[i.second]);
            }
        }
        if (intermediateSearch.empty()) return;
        // forming a response
        for (auto i = intermediateSearch.begin(); i != intermediateSearch.end(); i++) {
            searchResponse.insert(float(i.value())/max, i.key());
        }
    }
//get
    const QMultiMap<double,int> getSearchResponse(){
        return searchResponse;
    }

protected:
    QMultiMap<double,int> searchResponse;
};

class MainSearchEngine {
public:
//delete
    ~MainSearchEngine() {
        delete(history);
        delete(searchArchive);
    }
    MainSearchEngine (QString& path, QVector<QString>& format) {
        searchArchive = new DocumentBase(path, format);
    }
//write history (recording using QJson)
    void writeHistory() {
        QString val = to_string (history->getCollectHistory()).c_str();
        QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
        QFile responseRequest("answer.json");
        if (!responseRequest.open(QIODevice::WriteOnly)) {
            //error message
            QString errorMessage ("Error working with the answer file");
            errorLog(errorMessage);
            return;
        }
        responseRequest.write(QJsonDocument(doc).toJson(QJsonDocument::Indented));
        responseRequest.close();
    }
//multithreading
    static void dataOutput (History* staticHistory, DocumentBase* staticSearchArchive,
                           QString req, int idRequest) {
        SearchServer *searchServer = new SearchServer;
        searchServer->createresponce(req, staticSearchArchive);
//json
        auto &searchResult = searchServer->getSearchResponse();
        QString numRequest ("request");
        numRequest += QString("%1").arg(idRequest);
        mut.lock();
        staticHistory->setSearchEmpty(searchResult.empty(), numRequest.toStdString());
        mut.unlock();
        std::unordered_map<std::string, double> temp;
        for (auto a = searchResult.end(); a != searchResult.begin();) {
            --a;
            temp["docid"] = a.value();
//the json bug incorrectly perceives float 0.2
            temp["rank"] = (double)floorf(a.key()*1000) / 1000;
            mut.lock();
            staticHistory->setRecordingResponses(numRequest.toStdString(), temp);
            mut.unlock();
        }
        delete(searchServer);
    }
//get
    DocumentBase* getSearchArchive () {
        return searchArchive;
    }
    History* getHistory() {
        return history;
    }
protected:
    History* history = new History;
    DocumentBase* searchArchive = nullptr;
};

#endif // SEARCHENGINE_H
