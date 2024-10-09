#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QFile>
#include <QList>
#include <QHash>
#include <QDebug>
#include <QFuture>
#include <QVector>
#include <filesystem>
#include <QtConcurrent>
#include "errormessage.h"

#include "nlohmann/json.hpp"
#include "WI/word_indexing.h"

#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;


//functions for multithreading
json docIndexing (QString path, int id) {
    json indexing;
    indexing["id"] = id;
    indexing["address"] = path.toStdString();

    QFile resursec (path);
    if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
        resursec.close();
        errorLog("File reading error " + path, false);
        return "";
    }

    QString text = resursec.readAll();
    resursec.close();
    text = text.toLower();
    QHash<QString, int> word (WI::indexing_word(text, 1));
    for (auto i = word.begin(), end = word.end(); i != end; i++){
        indexing["index"][i.key().toStdString()] = i.value();
    }
    return indexing;
}
//FfM end

class DocumentBase {
public:
    DocumentBase (QString& path, QVector<QString>& format) {
        if (path.size() < 1) errorLog("Incorrect file path", true);
        fs::path directory (path.toStdString());
        QVector<QString> paths = search_extension(directory, format);

        //collecting the json search file
        int counter = 0;
        QList<QFuture<json>> informationResource;
        for (auto &p : paths) {
            informationResource.append(QtConcurrent::run(docIndexing, p, counter));
            counter++;
        }
        counter = 0;
        for (auto &w : informationResource) w.waitForFinished();
        for (auto &i : informationResource) {
            fileIndex["resources"][counter] = i.result();
            counter++;
        }
    }

    //collect files using the specified path
    QVector<QString> search_extension (fs::path& dir, QVector<QString>& ext) {
        QVector<QString> paths;
        for (const fs::directory_entry& p : fs::directory_iterator(dir)) {
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

protected:
    json fileIndex;

};

class SearchEngine : protected DocumentBase {
public:
    ~SearchEngine();
    SearchEngine (QString& rPath, QString& path, QVector<QString>& format): DocumentBase(path, format){
        requests << requestGeneration(rPath);
    }

    QVector<QString> requestGeneration (QString& path) {
        QFile reqFile(path);
        if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            errorLog("Search query file error", true);
        }
        json reqJson = json::parse(reqFile.readAll());
        if (reqJson["requests"].size() < 1) errorLog("The search query file is empty", true);

        QVector<QString> answer;
        for (auto &r : reqJson["requests"]){
            answer.append(QString::fromStdString(r));
        }

        return answer;
    }

private:
    QVector<QString> requests;
};

SearchEngine::~SearchEngine() {

}


#endif // WORKINGWITHJSON_H
