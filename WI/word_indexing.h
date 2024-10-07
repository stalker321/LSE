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

    QHash<QString, int> indexing_word (QString& resources, int minSize) {
        QTextCodec *codec = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForLocale(codec);
        QRegExp reg("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|-]+");
        QHash<QString, int> answer;
        for (auto &t : resources.split(reg, QString::SkipEmptyParts)) {
            if (t.size() > minSize) answer[t]++;
        }
        return answer;
    }
}

#endif // WORD_INDEXING_H
