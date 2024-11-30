#include "searchengine.h"

void SearchServer::createResponce (QString& req, DocumentBase* searchArchive) {
    auto temp = WordIndexing::indexingWord(req, stopWord);
//a blank to improve relevance
    QMultiMap<int, QString> reqIndex;
    for (auto i = temp.begin(), end = temp.end(); i != end; i++){
        reqIndex.insert(i.value(), i.key());
    }
    if (reqIndex.size() < 1) return;
//search
    QMap <int, int> intermediateSearch;
    int max = 0;
    for (auto r = reqIndex.begin(); r != reqIndex.end(); r++) {
        if (!searchArchive->getIndexBase().contains(r.value())) continue;
        for (auto &i : searchArchive->getIndexBase()[r.value()]) {
            intermediateSearch[i.second] += i.first;
            max = std::max(max,intermediateSearch[i.second]);
        }
    }
    if (intermediateSearch.empty()) return;
// forming a response
    int numberResponse (0);
    for (auto i = intermediateSearch.begin(); i != intermediateSearch.end(); i++) {
        if (numberResponse < numberOfResponses) numberResponse++;
        else break;
        searchResponse.insert(float(i.value())/max, i.key());
    }
}
