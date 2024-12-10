#include <wordIndexing.h>

#if QT_VERSION_MAJOR < 6
#include <QRegExp>
#else
#include <QRegularExpression>
using QRegExp = QRegularExpression;
#endif

QList<QString> uniqueWords (QString &resources, QList<QString> &stop_word) {
    static const QRegExp req ("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
    resources = resources.toLower();
    QList<QString> answer;
    for (auto &s : resources.split(req, Qt::SkipEmptyParts)) {
        if (!(answer.contains(s) || stop_word.contains(s))) answer.append(s);
    }
    return answer;
}

QList<QString> uniqueWords (QString &resources, QList<QString> word_list, QVector<QString>& stop_word) {
    static const QRegExp req ("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
    resources = resources.toLower();
    QString work = resources.toLower();
    for (auto &s : work.split(req, Qt::SkipEmptyParts)) {
        if (!(word_list.contains(s) || stop_word.contains(s))) word_list.append(s);
    }
    return word_list;
}

QHash<QString, int> indexingWord (QString& resources, QList<QString> &stop_word) {
    static const QRegExp req ("[\\s|\\n|\\r|\\.|,|!|\\?|:|;|«|»|•|…|-|—|-|–|(|)]+");
    resources = resources.toLower();
    QHash<QString, int> answer;
    for (auto &t : resources.split(req, Qt::SkipEmptyParts)){
        if (!stop_word.contains(t)) answer[t.toLower()]++;
    }
    return answer;
}
