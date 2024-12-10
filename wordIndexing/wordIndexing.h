#ifndef WORDINDEXING_H
#define WORDINDEXING_H

#include <QString>
#include <QList>
#include <QHash>

QList<QString> uniqueWords (QString &resources, QList<QString> &stop_word);
QList<QString> uniqueWords (QString &resources, QList<QString> word_list, QVector<QString>& stop_word);
QHash<QString, int> indexingWord (QString& resources, QList<QString> &stop_word);

#endif // WORDINDEXING_H
