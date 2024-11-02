#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H

#include "WorkingWithJson.h"

//database in json format
class DocumentBase {
public:
    ~DocumentBase() {
        delete(docBase);
    }
    DocumentBase (QString& path, QVector<QString>& format) {
        if (path.size() < 1) {
            errorLog("Incorrect file path", false);
            return;
        }
        fs::path directory (path.toStdString());
        QVector<QString> paths = searchExtension(directory, format);
        QList<QString> index;
        //collecting unique words
        for (auto &i : paths) {
            uniqueWords(i, index);
        }
//json work
        docBase->searchFile(paths, index);

    }

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

    void uniqueWords (QString& path, QList<QString>& index) {
            QFile resursec (path);
            if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
                resursec.close();
                errorLog("File reading error " + path, true);
            }

            QString info = resursec.readAll();
            info = info.toLower();
            resursec.close();
            std::list<std::string> answer;
            index = (WI::unique_words(info, index, stopWord));
            return;
    }


    const json getDocument () {
        return docBase->getWordIndexDatabase();
    }
protected:
//json
    Base* docBase = new Base;
};

struct SearchServer {
    void createresponce (QString& req, DocumentBase* searchArchive) {
        searchResponse.clear();
        auto temp = WI::indexing_word(req, stopWord);
        QMultiMap<int, QString> reqIndex;
        for (auto i = temp.begin(), end = temp.end(); i != end; i++){
            reqIndex.insert(i.value(), i.key());
        }
        if (reqIndex.size() < 1) return;
//search
        QList<QVector<int>> searchByIndex;
        for (auto r = reqIndex.begin(); r != reqIndex.end(); r++) {
            if (!searchArchive->getDocument()["index"].contains(r.value().toStdString())) continue;
            searchByIndex.append(searchResult(r.value(), searchArchive));
        }
        QVector<int> result(searchByIndex[0].size());
        for (auto& s : searchByIndex) {
            for (int i = 0; i < s.size(); i++) {
                result[i] += s[i];
            }
        }
        float max = *std::max_element(result.begin(), result.end());
//forming a response
        for(int i = 0; i < result.size(); i++) {
            if (result[i] == 0) continue;
            searchResponse.insert(result[i]/max, i);
        }
    }
//a useless function, but with fewer errors
    QVector<int> searchResult (QString search, DocumentBase* searchArchive) {
        QVector<int> answer = searchArchive->getDocument()["index"][search.toStdString()];
        return answer;
    }

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
    static void dataOutput (/*SearchServer* staticSearchServer*/ History* staticHistory,
                            DocumentBase* staticSearchArchive, QString& req, int idRequest) {
        SearchServer searchServer;
        searchServer.createresponce(req, staticSearchArchive);
        //json
        auto &searchResult = searchServer.getSearchResponse();
        QString numRequest ("request");
        numRequest += QString("%1").arg(idRequest);
        staticHistory->searchEmpty(searchResult.empty(), numRequest.toStdString());
        std::unordered_map<std::string, double> temp;
        for (auto a = searchResult.end(); a != searchResult.begin();) {
            --a;
            temp["docid"] = a.value();
//the json bug incorrectly perceives float 0.2
            temp["rank"] = (double)floorf(a.key()*1000) / 1000;
            staticHistory->recordingResponses(numRequest.toStdString(), temp);
        }
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
