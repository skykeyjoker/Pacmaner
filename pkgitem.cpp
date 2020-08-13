#include "pkgitem.h"

PkgItem::PkgItem(QString _pkgname,
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
                 QVector<QString> _groups):
    pkgname(_pkgname),
    pkgbase(_pkgbase),
    arch(_arch),
    pkgver(_pkgver),
    pkgrel(_pkgrel),
    epoch(_epoch),
    pkgdesc(_pkgdesc),
    url(_url),
    maintainer(_maintainer),
    licenses(_licenses),
    conflicts(_conflicts),
    provides(_provides),
    replaces(_replaces),
    depends(_depends),
    optdepends(_optdepends),
    makedepends(_makedepends),
    checkdepends(_checkdepends),
    groups(_groups)
{

}

void PkgItem::printInfo()
{
    qDebug()<<"[PkgItem Info]";
    qDebug()<<pkgname;
    qDebug()<<pkgbase;
    qDebug()<<arch;
    qDebug()<<pkgver;
    qDebug()<<pkgrel;
    qDebug()<<epoch;
    qDebug()<<pkgdesc;
    qDebug()<<url;
    qDebug()<<maintainer;
    qDebug()<<licenses;
    qDebug()<<conflicts;
    qDebug()<<provides;
    qDebug()<<replaces;
    qDebug()<<depends;
    qDebug()<<optdepends;
    qDebug()<<makedepends;
    qDebug()<<checkdepends;
}

bool PkgItem::isAurPkg()
{
    return false;
}
