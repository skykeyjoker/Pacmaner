#ifndef QUERYMODE_H
#define QUERYMODE_H

#include <QMetaType>

enum QueryMode{OnlyOfficial,OnlyAur,OfficialAndAur,AurInfo};

Q_DECLARE_METATYPE(QueryMode)

#endif // QUERYMODE_H
