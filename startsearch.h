#ifndef STARTSEARCH_H
#define STARTSEARCH_H

#include <QString>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

QString configCheck (QString config, QString currentVersion);
json search_query (QString& path);

#endif // STARTSEARCH_H
