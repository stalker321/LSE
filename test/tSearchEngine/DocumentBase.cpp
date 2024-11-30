#include "searchengine.h"

std::mutex baseMut;

//multithreading
FileInfo docIndexing (QString path, int id) {
    FileInfo indexing;
    indexing.address = path;
    indexing.id = id;
    QFile resursec (path);

    if (!resursec.open(QIODevice::ReadOnly | QIODevice::Text)){
        resursec.close();
        errorLog(("File reading error " + path), true);
    }

    QString text = resursec.readAll();
    resursec.close();
    indexing.textIndex = WordIndexing::indexingWord(text, stopWord);
    return indexing;
}

//collect files using the specified path
void filePath (fs::directory_entry p, QList<QString>& ext, QVector<QString>& path){
    for (auto& e : ext) {
        if (!p.path().extension().compare(e.toStdString())){
            path.append(QString::fromStdString(p.path().u8string()));
            break;
        }
    }
}

QVector<QString> searchExtension (fs::path dir, QList<QString>& ext) {
    QVector<QString> paths;
    if (fs::is_regular_file(dir)) {
        paths.append(QString::fromStdString(fs::directory_entry(dir).path().u8string()));
        return paths;
    };
    for (const fs::directory_entry& p : fs::directory_iterator(dir)){
        if (!fs::is_regular_file(p.status())) continue;
        filePath(p, ext, paths);
    }
    return paths;
}

void writeIndex (FileInfo info, QMap<QString, QVector<QPair<int, int>>> *base) {
    if (!info.textIndex.empty()){
        for (auto i = info.textIndex.begin(); i != info.textIndex.end(); i++) {
            baseMut.lock();
            (*base)[i.key()].append(QPair<int, int>(i.value(), info.id));
            baseMut.unlock();
        }
    }
}

void searchFile(QVector<QString>& filePaths, int& id, QVector<FileInfo>& docBase) {
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

//add docBase
void DocumentBase::setBase (QString& path, QList<QString>& format) {
    QVector<QString> paths;
    if (!fs::exists(path.toStdString())) {
        errorLog("Incorrect file path", false);
        return;
    }
    paths.append(searchExtension(fs::path(path.toStdString()), format));
    if (paths.empty()) {
        errorLog("Incorrect file path", false);
        return;
    }
    searchFile(paths, id, docBase);
    QList<std::thread*> readIndexBase;
    for (auto &i : docBase) {
        readIndexBase.append(new std::thread(writeIndex, std::move(i), &indexBase));
        i.textIndex.clear();
    }
    for (auto *i : readIndexBase) i->join();
}

//doc base
DocumentBase::DocumentBase (QList<QString>& path, QList<QString>& format) {
    QVector<QString> paths;
    for(auto &p : path) {
        if (!fs::exists(p.toStdString())) {
            errorLog("Incorrect file path", false);
            continue;
        }
        paths.append(searchExtension(fs::path(p.toStdString()), format));
    }
    if (paths.empty()) {
        errorLog("Incorrect file path", true);
        return;
    }
    searchFile(paths, id, docBase);
    QList<std::thread*> readIndexBase;
    for (auto &i : docBase) {
        readIndexBase.append(new std::thread(writeIndex, std::move(i), &indexBase));
        i.textIndex.clear();
    }
    for (auto *i : readIndexBase) i->join();
}


DocumentBase::~DocumentBase() {}
