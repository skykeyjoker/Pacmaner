#ifndef QUERYMODE_H
#define QUERYMODE_H

#include <QMetaType>

typedef enum QueryMode{OnlyOfficial,OnlyAur,OfficialAndAur,AurInfo}QueryMode;

Q_DECLARE_METATYPE(QueryMode)

#endif // QUERYMODE_H
