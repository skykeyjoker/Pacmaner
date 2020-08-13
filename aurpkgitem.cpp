#include "aurpkgitem.h"

AurPkgItem::AurPkgItem(QString _pkgname,
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
                       QVector<QString> _keywords)
    :PkgItem(_pkgname,
             _pkgbase,
             _arch,
             _pkgver,
             _pkgrel,
             _epoch,
             _pkgdesc,
             _url,
             _maintainer,
             _licenses,
             _conflicts,
             _provides,
             _replaces,
             _depends,
             _optdepends,
             _makedepends,
             _checkdepends,
             _groups),
      id(_id),
      packageBaseId(_packageBaseId),
      numVotes(_numVotes),
      popularity(_popularity),
      outOfDate(_outOfDate),
      firstSubmitted(_firstSubmitted),
      lastModified(_lastModified),
      urlPath(_urlPath),
      keywords(_keywords)
{

}

void AurPkgItem::printInfo()
{
    qDebug()<<"[Aur PkgItem]";
    PkgItem::printInfo();
    qDebug()<<id;
    qDebug()<<packageBaseId;
    qDebug()<<numVotes;
    qDebug()<<outOfDate;
    qDebug()<<firstSubmitted;
    qDebug()<<lastModified;
    qDebug()<<urlPath;
}

bool AurPkgItem::isAurPkg()
{
    return true;
}
