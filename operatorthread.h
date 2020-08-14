#ifndef OPERATORTHREAD_H
#define OPERATORTHREAD_H

#include <QObject>
#include <QProcess>
#include "operation.h"
#include "OperationMode.h"

class OperatorThread : public QObject
{
    Q_OBJECT
public:
    explicit OperatorThread(QObject *parent = nullptr);

signals:
    void operationFinished(Operation *operation);
    void operationFailed(const QString &pkgname, const QByteArray &err);
    void allOperationFinished();
public slots:
    void startOperator(const QVector<Operation*> &_operations, const QString &_password);

};

#endif // OPERATORTHREAD_H
