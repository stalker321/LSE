#ifndef WORKINGWITHJSON_H
#define WORKINGWITHJSON_H
#include <QFile>
#include <QDebug>
#include <filesystem>
#include <QVector>

#include "nlohmann/json.hpp"

#include <iostream>

using json = nlohmann::json;
namespace fs = std::filesystem;

class SearchEngine {
public:
private:
};

class DocumentBase {
public:
    ~DocumentBase();
    DocumentBase (QString path, QString format) {

        fs::path directory (path.toStdString());
        std::string extension (format.toStdString());
        search_extension(directory, extension);
    }

    void search_extension (fs::path& dir, std::string& ext) {
        for (auto& p : fs::directory_iterator(dir)) {
            if (!fs::is_regular_file(p.status())) continue;
            if (!p.path().extension().compare(ext)) qDebug() << p.path().u8string().c_str();
        }
    }

private:
protected:
    json fileIndex;

};

DocumentBase::~DocumentBase() {

}

#endif // WORKINGWITHJSON_H
