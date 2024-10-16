#ifndef WORD_INDEXING_H
#define WORD_INDEXING_H

#include <QTextCodec>
#include <QString>
#include <QList>
#include <QHash>

namespace WI {
    int word_count (QString &resources, QString &word) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|-]+");
        if (word == " ") return resources.split(reg, QString::SkipEmptyParts).count();
        return resources.split(word).count() - 1;
    }

    QList<QString> list_world (QString &resources) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QList<QString> answer;
        for (auto &s : resources.split(reg, QString::SkipEmptyParts)) {
            answer.append(s);
        }
        return answer;
    }

    QList<QString> unique_words (QString &resources) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QList<QString> answer;
        for (auto &s : resources.split(reg, QString::SkipEmptyParts)) {
            if (!answer.contains(s)) answer.append(s);
        }
        return answer;
    }

    QList<QString> unique_words (QString &resources, QList<QString> word_list) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QString work = resources.toLower();
        for (auto &s : work.split(reg, QString::SkipEmptyParts)) {
            if (!word_list.contains(s)) word_list.append(s);
        }
        return word_list;
    }

    QList<QString> unique_words (QString &resources, QList<QString> word_list, int minSize) {
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QString work = resources.toLower();
        for (auto &s : work.split(reg, QString::SkipEmptyParts)) {
            if (!(word_list.contains(s)) && s.size() > minSize) word_list.append(s);
        }
        return word_list;
    }

    QHash<QString, int> indexing_word (QString& resources, int minSize) {
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QHash<QString, int> answer;
        for (auto &t : resources.split(reg, QString::SkipEmptyParts)) {
            if (t.size() > minSize) answer[t.toLower()]++;
        }
        return answer;
    }
}

#endif // WORD_INDEXING_H
