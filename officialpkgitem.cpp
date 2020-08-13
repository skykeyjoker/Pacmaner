#include "officialpkgitem.h"

OfficialPkgItem::OfficialPkgItem(QString _pkgname,
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
                                 QString _repo,
                                 QString _filename,
                                 int _compressed_size,
                                 int _installed_size,
                                 QString _build_date,
                                 QString _last_update,
                                 QString _flag_date,
                                 QString _packager)
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
      repo(_repo),
      filename(_filename),
      compressed_size(_compressed_size),
      installed_size(_installed_size),
      build_date(_build_date),
      last_update(_last_update),
      flag_date(_flag_date),
      packager(_packager)
{

}

void OfficialPkgItem::printInfo()
{
    qDebug()<<"[OfficialPkgItem]";
    PkgItem::printInfo();
    qDebug()<<repo;
    qDebug()<<filename;
    qDebug()<<compressed_size;
    qDebug()<<installed_size;
    qDebug()<<build_date;
    qDebug()<<last_update;
    qDebug()<<flag_date;
    qDebug()<<packager;
    qDebug()<<groups;
}

bool OfficialPkgItem::isAurPkg()
{
    return false;
}
