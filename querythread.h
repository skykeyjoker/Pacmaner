#ifndef QUERYTHREAD_H
#define QUERYTHREAD_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonObject>
#include <QJsonArray>

#include <QMetaType>


#include <QDeadlineTimer>

#include "querymode.h"
#include "matchmode.h"
#include "officialpkgitem.h"
#include "aurpkgitem.h"

class QueryThread : public QObject
{
    Q_OBJECT
public:
    explicit QueryThread(QObject *parent = nullptr);

    void splitResults(QNetworkReply*, QueryMode);

private:
    QNetworkAccessManager *manager;
    QNetworkRequest request;
    QVector<PkgItem*> result;
    int _count;
signals:
    void queryStarted();
    void queryFinished(QVector<PkgItem*>);

public slots:
    void startQuery(QString target, QueryMode queryMode, MatchMode matchMode);

};

#endif // QUERYTHREAD_H
