#ifndef OPERATION_H
#define OPERATION_H

#include <QtCore>
#include "OperationMode.h"

class Operation
{
public:
    Operation();
    Operation(const QString &_pkgname, OperationMode _mode, bool _isAur):
        pkgname(_pkgname), mode(_mode), isAur(_isAur)
    {}

    OperationMode mode;
    QString pkgname;
    bool isAur;
};

#endif // OPERATION_H
