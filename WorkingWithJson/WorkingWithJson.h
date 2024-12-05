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
#include <QJsonArray>
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "errormessage.h"

extern QList<QString> stopWord;

//working with json config
class SistemJson {
public:
    ~SistemJson();
//config
    void setConfigCheck (QString& config, QString& currentVersion);
//get
    const QJsonDocument &getInfo() {
        return info;
    }
    void setMaxResponsec(int max);
protected:
    QJsonDocument info;
    QString path;
};

//working with json request
struct RequestJson{
    //query
    void setSearchQuery (QString& path);
    //get
    const QJsonDocument &getRequests() {
        return requests;
    }
protected:
    QJsonDocument requests;
};

//working with json blacklist
struct BlacklistOfWords {
    void setBlackList(QString& path);
    ~BlacklistOfWords();
//get
    const QJsonDocument&getBlacklist(){
        return blacklist;
    }
protected:
    QString filePath;
    QJsonDocument blacklist;
};

//response history
class History {
public:
    void setSearchEmpty (bool empty, QString request);
    void setRecordingResponses (QString request,
                               QMultiMap<double, int> result);
    void write ();
    QJsonDocument getAnswer (QString req);
//get
    const QJsonObject &getCollectHistory() {
        return collectHistory;
    }
private:
    QJsonObject collectHistory;
};

#endif // WORKINGWITHJSON_H
