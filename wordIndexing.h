#ifndef WORDINDEXING_H
#define WORDINDEXING_H

#include <QString>
#include <QList>
#include <QHash>

#if QT_VERSION_MAJOR < 6
    #include <QRegExp>
#else
    #include <QRegularExpression>
    using QRegExp = QRegularExpression;
#endif

struct WordIndexing {

    static QList<QString> uniqueWords (QString &resources, QVector<QString> &stop_word) {
        resources = resources.toLower();
        QList<QString> answer;
        for (auto &s : resources.split(QRegExp("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+"), Qt::SkipEmptyParts)) {
            if (!(answer.contains(s) || stop_word.contains(s))) answer.append(s);
        }
        return answer;
    }

    static QList<QString> uniqueWords (QString &resources, QList<QString> word_list, QVector<QString>& stop_word) {
        resources = resources.toLower();
        QString work = resources.toLower();
        for (auto &s : work.split(QRegExp("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+"), Qt::SkipEmptyParts)) {
            if (!(word_list.contains(s) || stop_word.contains(s))) word_list.append(s);
        }
        return word_list;
    }

    static QHash<QString, int> indexingWord (QString& resources, QVector<QString> &stop_word) {
        resources = resources.toLower();
        QHash<QString, int> answer;
        for (auto &t : resources.split(QRegExp("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+"), Qt::SkipEmptyParts)){
            if (!stop_word.contains(t)) answer[t.toLower()]++;
        }
        return answer;
    }
};

#endif // WORDINDEXING_H
