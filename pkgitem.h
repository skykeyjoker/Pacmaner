#ifndef PKGITEM_H
#define PKGITEM_H

#include <QtCore>
#include <QDebug>

class PkgItem
{
public:
    PkgItem();
    PkgItem(QString _pkgname,
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
            QVector<QString> _groups);

    virtual void printInfo();
    virtual bool isAurPkg();

    QString pkgname;
    QString pkgbase;
    QString arch;
    QString pkgver;
    int pkgrel;
    int epoch;
    QString pkgdesc;
    QString url;
    QVector<QString> maintainer;
    QVector<QString> licenses;
    QVector<QString> conflicts;
    QVector<QString> provides;
    QVector<QString> replaces;
    QVector<QString> depends;
    QVector<QString> optdepends;
    QVector<QString> makedepends;
    QVector<QString> checkdepends;
    QVector<QString> groups;
};

Q_DECLARE_METATYPE(PkgItem)

#endif // PKGITEM_H
