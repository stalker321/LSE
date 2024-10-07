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
        return "";
    }

    QString text = resursec.readAll();
    resursec.close();
    text = text.toLower();
    qDebug() << text;
    QHash<QString, int> word (WI::indexing_word(text, 1));
    for (auto i = word.begin(), end = word.end(); i != end; i++){
        indexing["index"][i.key().toStdString()] = i.value();
    }
    return indexing;
}
//FfM end

class SearchEngine {
public:
    ~SearchEngine();
private:
};

SearchEngine::~SearchEngine() {

}

class DocumentBase {
public:
    DocumentBase (QString& path, QVector<QString>& format) {
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
        qInfo() << to_string(fileIndex).c_str();
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

private:
    json fileIndex;
protected:

};

#endif // WORKINGWITHJSON_H
