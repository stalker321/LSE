#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QMap>
#include <QFile>
#include <QList>
#include <QHash>
#include <QDebug>
#include <QFuture>
#include <QVector>
#include <QMultiMap>
#include <filesystem>
#include <QtConcurrent>
#include "errormessage.h"

#include "nlohmann/json.hpp"
#include "WI/word_indexing.h"

#include <iostream>
#include <list>
#include <string>

using json = nlohmann::json;
namespace fs = std::filesystem;

QVector<QString> stop_word;

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
//        qDebug() << to_string(fileIndex["index"]).c_str();
//        qDebug() << to_string(fileIndex["address"]).c_str();
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
        QMultiMap<int, QString> req_index;
        for (auto i = temp.begin(), end = temp.end(); i != end; i++){
            req_index.insert(i.value(), i.key());
        }
        //mult
        QList<QFuture<QVector<int>>> multiple_counting;
        for (auto i = req_index.begin(); i != req_index.end(); i++) {
            if (!database_index["index"].contains(i.value().toStdString())) continue;
            multiple_counting.append(QtConcurrent::run(searchResult, i.value(), database_index));
        }

        for (auto& i : multiple_counting) i.waitForFinished();
        QVector<int> result(multiple_counting[0].result().size());
        for (auto& i : multiple_counting) {
            for (int v = 0; v < i.result().size(); v++) {
                result[v] = i.result()[v];
            }
        }
    }

    static QVector<int> searchResult (QString search, json& database) {
        QVector<int> answer = database["index"][search.toStdString()];
        return answer;
    }
};

class MainEngine {
public:
    static void dataOutput (QString& req, json &database_index) {
        SearchServer(req, database_index);
    }
private:
};

#endif // WORKINGWITHJSON_H
