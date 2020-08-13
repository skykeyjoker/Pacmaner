#ifndef AURPKGITEM_H
#define AURPKGITEM_H

#include "pkgitem.h"

class AurPkgItem : public PkgItem
{
public:
    AurPkgItem();
    AurPkgItem(QString _pkgname,
               QString _pkgbase,
               QString _arch,
               QString _pkgver,
               int _pkgrel,
               int _epoch,
               QString _pkgdesc,
               QString _url,
               QVector<QString> _maintainer,
               QVector<QString> _licenses,
               QVector<QString> _conflicts,
               QVector<QString> _provides,
               QVector<QString> _replaces,
               QVector<QString> _depends,
               QVector<QString> _optdepends,
               QVector<QString> _makedepends,
               QVector<QString> _checkdepends,
               QVector<QString> _groups,
               qlonglong _id,
               qlonglong _packageBaseId,
               int _numVotes,
               double _popularity,
               QString _outOfDate,
               qlonglong _firstSubmitted,
               qlonglong _lastModified,
               QString _urlPath,
               QVector<QString> _keywords);

    virtual void printInfo();
    virtual bool isAurPkg();

    qlonglong id;
    qlonglong packageBaseId;
    int numVotes;
    double popularity;
    QString outOfDate;
    qlonglong firstSubmitted;
    qlonglong lastModified;
    QString urlPath;
    QVector<QString> keywords;
};

#endif // AURPKGITEM_H
