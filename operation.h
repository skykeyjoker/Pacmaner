#ifndef OPERATION_H
#define OPERATION_H

#include <QtCore>
#include "OperationMode.h"

class Operation
{
public:
    Operation();
    Operation(const QString &_pkgname, OperationMode _mode):
        pkgname(_pkgname), mode(_mode)
    {}

    OperationMode mode;
    QString pkgname;
};

#endif // OPERATION_H
