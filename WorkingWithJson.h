#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QFile>
#include <QList>
#include <QDebug>
#include <QFuture>
#include <QVector>
#include <filesystem>
#include <QtConcurrent>

#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;


QString filePath (fs::directory_entry p, QVector<QString> ext){
    QString path = "";
    for (auto& e : ext) {
        if (p.path().extension().compare(e.toStdString())){
            path = QString::fromStdString(p.path().u8string());
            break;
        }
    }
    return path;
}

class SearchEngine {
public:
private:
};

class DocumentBase {
public:
    ~DocumentBase();
    DocumentBase (QString path, QVector<QString> format) {
        fs::path directory (path.toStdString());
        QString paths = search_extension(directory, format);
    }

    QString search_extension (fs::path& dir, QVector<QString>& ext) {
        QString paths;
        QList<QFuture<QString>> findingResources;
        for (const fs::directory_entry& p : fs::directory_iterator(dir)) {
            if (!fs::is_regular_file(p.status())) continue;
            //multithreaded search
            findingResources.append(QtConcurrent::run(filePath, p, ext));
        }
        //
        for (auto &t : findingResources) t.waitForFinished();
        for (auto &f : findingResources) paths += f.result() + '\n';

        return paths;
    }

    json docIndexing (QString& path, int id) {
        json indexing;
        return indexing;
    }

private:
protected:
    json fileIndex;

};

DocumentBase::~DocumentBase() {

}

#endif // WORKINGWITHJSON_H
