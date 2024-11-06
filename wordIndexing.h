#ifndef WORDINDEXING_H
#define WORDINDEXING_H

#include <QTextCodec>
#include <QString>
#include <QList>
#include <QHash>


struct WordIndexing {
// unused
//    int word_count (QString &resources, QString &word) {
//        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|-|—]+");
//        if (word == " ") return resources.split(reg, QString::SkipEmptyParts).count();
//        return resources.split(word).count() - 1;
//    }

//    QList<QString> list_world (QString &resources, QVector<QString> &stop_word ) {
//        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
//        resources = resources.toLower();
//        QList<QString> answer;
//        for (auto &s : resources.split(reg, QString::SkipEmptyParts)) {
//            if (!stop_word.contains(s)) answer.append(s);
//        }
//        return answer;
//    }

    static QList<QString> unique_words (QString &resources, QVector<QString> &stop_word) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
        resources = resources.toLower();
        QList<QString> answer;
        for (auto &s : resources.split(reg, QString::SkipEmptyParts)) {
            if (!(answer.contains(s) || stop_word.contains(s))) answer.append(s);
        }
        return answer;
    }

    static QList<QString> unique_words (QString &resources, QList<QString> word_list, QVector<QString>& stop_word) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
        resources = resources.toLower();
        QString work = resources.toLower();
        for (auto &s : work.split(reg, QString::SkipEmptyParts)) {
            if (!(word_list.contains(s) || stop_word.contains(s))) word_list.append(s);
        }
        return word_list;
    }

    static QHash<QString, int> indexing_word (QString& resources, QVector<QString> &stop_word) {
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        resources = resources.toLower();
        QTextCodec::setCodecForLocale(codec);
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
        QHash<QString, int> answer;
        for (auto &t : resources.split(reg, QString::SkipEmptyParts)){
            if (!stop_word.contains(t)) answer[t.toLower()]++;
        }
        return answer;
    }
};

#endif // WORDINDEXING_H
