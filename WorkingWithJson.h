#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QMap>
#include <math.h>
#include <QFile>
#include <QList>
#include <QHash>
#include <QDebug>
#include <QFuture>
#include <QVector>
#include <QMultiMap>
#include <QByteArray>
#include <filesystem>
#include <QtConcurrent>
#include "errormessage.h"

#include "nlohmann/json.hpp"
#include "WI/word_indexing.h"

#include <string>


using json = nlohmann::json;
namespace fs = std::filesystem;

QVector<QString> stop_word;
QMutex read_answer;
json history;

//database in json format
class DocumentBase {
public:
    DocumentBase (QString& path, QVector<QString>& format) {
        if (path.size() < 1) errorLog("Incorrect file path", true);
        fs::path directory (path.toStdString());
        QVector<QString> paths = search_extension(directory, format);
        QList<QString> index;
        int counter = 0;
        //collecting unique words
        for (auto &i : paths) {
            uniqueWords(i, index);
        }
        for (auto &i : index) fileIndex["index"][i.toStdString()].push_back(0);

        //collecting the json search file
        QList<QFuture<json>> informationResource;
        for (auto &p : paths) {
            informationResource.append(QtConcurrent::run(docIndexing, p));
        }
        for (auto &w : informationResource) w.waitForFinished();
        for (auto &i : informationResource) {
            fileIndex["address"].push_back(i.result()["address"]);

            json temp = i.result()["index"];
            for (auto t = temp.begin(); t != temp.end(); t++) {
                if (counter == 0) {
                     fileIndex["index"][t.key()][counter] = (t.value());
                } else {
                    fileIndex["index"][t.key()].push_back(t.value());
                }
            }

            if (counter == 0) {
                counter++;
                continue;
            }

            for (auto &t : fileIndex["index"]){
                if (t[counter].empty()) t[counter] = 0;
            }
            counter++;
        }
    }

    //collect files using the specified path
    QVector<QString> search_extension (fs::path& dir, QVector<QString>& ext) {
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

    const json &get_fileIndex () {
        return fileIndex;
    }

    //functions for multithreading
    static json docIndexing (QString path) {
        json indexing;
        indexing["address"] = path.toStdString();

        QFile resursec (path);
        if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
            resursec.close();
            errorLog("File reading error " + path, true);
        }

        QString text = resursec.readAll();
        resursec.close();
        QHash<QString, int> word (WI::indexing_word(text, stop_word));
        for (auto i = word.begin(), end = word.end(); i != end; i++){
            indexing["index"][i.key().toStdString()] = i.value();
        }
        return indexing;
    }

    static void uniqueWords (QString& path, QList<QString>& index) {
            QFile resursec (path);
            if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
                resursec.close();
                errorLog("File reading error " + path, true);
            }

            QString info = resursec.readAll();
            info = info.toLower();
            resursec.close();
            std::list<std::string> answer;
            index = (WI::unique_words(info, index, stop_word));
            return;
    };


protected:
    json fileIndex;

};

class SearchServer {
public:
    SearchServer (QString& req, json &database_index) {
        auto temp = WI::indexing_word(req, stop_word);
        QMultiMap<int, QString> reqIndex;
        for (auto i = temp.begin(), end = temp.end(); i != end; i++){
            reqIndex.insert(i.value(), i.key());
        }
        if (reqIndex.size() < 1) return;

//search
        QList<QVector<int>> searchByIndex;
        for (auto r = reqIndex.begin(); r != reqIndex.end(); r++) {
            if (!database_index["index"].contains(r.value().toStdString())) continue;
            searchByIndex.append(searchResult(r.value(), database_index));
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
            search_response.insert(result[i]/max, i);
        }
    }
//a useless function, but with fewer errors
    QVector<int> searchResult (QString search, json& database) {
        QVector<int> answer = database["index"][search.toStdString()];
        return answer;
    }

    const QMultiMap<double,int> get_search_response(){
        return search_response;
    }

private:
    QMultiMap<double,int> search_response;
};

struct MainEngine {
    static void dataOutput (QString& req, json &database_index, int request_id) {
        SearchServer searchServer (req, database_index);
        //json
        auto &searchResult = searchServer.get_search_response();
        QString num_request ("request");
        num_request += QString("%1").arg(request_id);
        if (searchResult.empty()) {
            history["answer"][num_request.toStdString()]["result"] = false;
        } else {
            history["answer"][num_request.toStdString()]["result"] = true;
        }
        json test;
        for (auto a = searchResult.end(); a != searchResult.begin();) {
            --a;
            test["docid"] = a.value();
            //the json bug incorrectly perceives float 0.2
            test["rank"] = (double)floorf(a.key()*1000) / 1000;
            history["answer"][num_request.toStdString()]["relevance"]["rank"].emplace_back(test);
        }
    }

    static void write_history () {
        QString val = to_string (history).c_str();
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
};

#endif // WORKINGWITHJSON_H
