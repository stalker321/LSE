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
#include <QtConcurrent>
#include "errormessage.h"


//working with json config/query files
struct SistemJson {
//config
    void setConfigCheck (QString config, QString currentVersion);
//query
    void setSearchQuery (QString& path);
//get
    const QJsonDocument &getInfo() {
        return info;
    }
protected:
    QJsonDocument info;
};
//response history
class History {
public:
    void setSearchEmpty (bool empty, QString request);
    void setRecordingResponses (QString request,
                               QMultiMap<double, int> result);
    void write ();
//get
    const QJsonObject &getCollectHistory() {
        return collectHistory;
    }

private:
    QJsonObject collectHistory;
};

#endif // WORKINGWITHJSON_H
