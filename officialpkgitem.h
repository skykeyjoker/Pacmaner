#ifndef OFFICIALPKGITEM_H
#define OFFICIALPKGITEM_H

#include "pkgitem.h"

class OfficialPkgItem : public PkgItem
{
public:
    OfficialPkgItem();
    OfficialPkgItem(QString _pkgname,
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
                    QString _packager);

    virtual void printInfo();
    virtual bool isAurPkg();

    QString repo;
    QString filename;
    int compressed_size;
    int installed_size;
    QString build_date;
    QString last_update;
    QString flag_date;
    QString packager;
};

#endif // OFFICIALPKGITEM_H
