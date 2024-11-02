#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QMap>
#include <QFile>
#include <QList>
#include <QHash>
#include <math.h>
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

QVector<QString> stopWord;

using json = nlohmann::json;
namespace fs = std::filesystem;

//working with json config/query files
struct SistemJson {
//config
    void configCheck (QString config, QString currentVersion) {
        QFile fileConfig (config);
        if (!fileConfig.open(QIODevice::ReadOnly | QIODevice::Text)) {
            fileConfig.close();
            errorLog("error in the config file", true);
        }
        info = json::parse(fileConfig.readAll().toStdString());
        if (info["config"]["version"] != currentVersion.toStdString()) {
            fileConfig.close();
            errorLog("mismatch of project versions", true);
        }

        fileConfig.close();
    }
//query
    void searchQuery (QString& path) {
        QFile reqFile(path);
        if (!reqFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            reqFile.close();
            errorLog("Search query file error", true);
        }
        info = json::parse(reqFile.readAll());
        reqFile.close();
        if (info["requests"].size() < 1) errorLog("The search query file is empty", true);
    }
//get
        const json &getInfo() {
            return info;
        }
protected:
    json info;
};
//document base
class Base {
public:
    void searchFile (QVector<QString>& filePaths, QList<QString>& index) {
        for (auto &i : index) wordIndexDatabase["index"][i.toStdString()].push_back(0);
        int counter = 0;
        QList<QFuture<json>> informationResource;
        for (auto &p : filePaths) {
            informationResource.append(QtConcurrent::run(docIndexing, p));
        }
        for (auto &w : informationResource) w.waitForFinished();
        for (auto &i : informationResource) {
            wordIndexDatabase["address"].push_back(i.result()["address"]);
            json temp = i.result()["index"];
            for (auto t = temp.begin(); t != temp.end(); t++) {
                if (counter == 0) {
                    wordIndexDatabase["index"][t.key()][counter] = (t.value());
                } else {
                    wordIndexDatabase["index"][t.key()].push_back(t.value());
                }
            }
            if (counter == 0) {
                counter++;
                continue;
            }

            for (auto &t : wordIndexDatabase["index"]){
                if (t[counter].empty()) t[counter] = 0;
            }
            counter++;
        }
    }
//multithreading
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
        QHash<QString, int> word (WI::indexing_word(text, stopWord));
        for (auto i = word.begin(), end = word.end(); i != end; i++){
            indexing["index"][i.key().toStdString()] = i.value();
        }
        return indexing;
    }
//get
    const json &getWordIndexDatabase() {
        return wordIndexDatabase;
    }

private:
    json wordIndexDatabase;
};
//response history
class History {
public:
    void searchEmpty (bool empty, std::string request) {
        if (empty) collectHistory["answer"][request]["result"] = false;
        else collectHistory["answer"][request]["result"] = true;
    }

    void recordingResponses (std::string request,
        std::unordered_map<std::string, double> rec) {
        collectHistory["answer"][request]["relevance"].push_back(rec);
    }

//get
    const json &getCollectHistory() {
        return collectHistory;
    }

private:
    json collectHistory;
};

#endif // WORKINGWITHJSON_H
