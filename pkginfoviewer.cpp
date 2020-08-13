#include "pkginfoviewer.h"

PkgInfoViewer::PkgInfoViewer(PkgItem* ite, QWidget *parent) : QMainWindow(parent)
{
    // Windows
    setWindowTitle(tr("%1 包信息查看").arg(ite->pkgname));
    setWindowIcon(QIcon(":/icons/pkginfo.png"));

    // Menu
    QMenuBar *menubar = new QMenuBar;
    setMenuBar(menubar);

    QMenu *menu = menubar->addMenu("操作");
    QAction *act_ins = new QAction("安装该软件包");
    QAction *act_uins = new QAction("卸载该软件包");
    QAction *act_exit = new QAction("退出");

    menu->addAction(act_ins);
    menu->addAction(act_uins);
    menu->addAction(act_exit);


    // Init UI
    QWidget *centerWidget = new QWidget;
    this->setCentralWidget(centerWidget);
    mainLay = new QVBoxLayout(centerWidget);

    // Base Group
    QGroupBox *groupBase = new QGroupBox("基本信息");
    QGridLayout *glay_groupBase = new QGridLayout(groupBase);

    glay_groupBase->addWidget(new QLabel("Pkgname:"),0,0);
    glay_groupBase->addWidget(new QLabel(ite->pkgname),0,1);

    glay_groupBase->addWidget(new QLabel("Pkgbase:"),0,3);
    glay_groupBase->addWidget(new QLabel(ite->pkgbase),0,4);

    glay_groupBase->addWidget(new QLabel("Arch:"),1,0);
    glay_groupBase->addWidget(new QLabel(ite->arch),1,1);

    glay_groupBase->addWidget(new QLabel("Pkgver:"),2,0);
    glay_groupBase->addWidget(new QLabel(ite->pkgver),2,1);

    glay_groupBase->addWidget(new QLabel("Pkgrel:"),2,3);
    glay_groupBase->addWidget(new QLabel(QString::number(ite->pkgrel)),2,4);

    glay_groupBase->addWidget(new QLabel("Epoch:"),3,0);
    glay_groupBase->addWidget(new QLabel(QString::number(ite->epoch)),3,1);

    glay_groupBase->addWidget(new QLabel("Url:"),4,0);
    glay_groupBase->addWidget(new QLabel(ite->url),4,1,1,2);

    glay_groupBase->addWidget(new QLabel("Maintainer:"),5,0);
    QLabel *label_maintainer = new QLabel;
    QString _maintainer;
    for(auto i : ite->maintainer)
    {
        _maintainer += (i + " ");
    }
    label_maintainer->setText(_maintainer);
    glay_groupBase->addWidget(label_maintainer,5,1,1,4);


    glay_groupBase->addWidget(new QLabel("License:"),6,0);
    QLabel *label_license = new QLabel;
    QString _license;
    for(auto i : ite->licenses)
    {
        _license += (i + " ");
    }
    label_license->setText(_license);
    glay_groupBase->addWidget(label_license,6,1,1,4);

    glay_groupBase->addWidget(new QLabel("Conflicts:"),7,0);
    QLabel *label_conflict = new QLabel;
    QString _conflict;
    for(auto i : ite->conflicts)
    {
        _conflict += (i + " ");
    }
    label_conflict->setText(_conflict);
    glay_groupBase->addWidget(label_conflict,7,1,1,4);

    glay_groupBase->addWidget(new QLabel("Provides:"),8,0);
    QLabel *label_provide = new QLabel;
    QString _provide;
    for(auto i : ite->provides)
    {
        _provide += (i + " ");
    }
    label_provide->setText(_provide);
    glay_groupBase->addWidget(label_provide,8,1,1,4);

    glay_groupBase->addWidget(new QLabel("Replaces:"),9,0);
    QLabel *label_replace = new QLabel;
    QString _replace;
    for(auto i : ite->replaces)
    {
        _replace += (i + " ");
    }
    label_provide->setText(_replace);
    glay_groupBase->addWidget(label_replace,9,1,1,4);

    glay_groupBase->addWidget(new QLabel("Depends:"),10,0);
    QLabel *label_depend = new QLabel;
    QString _depend;
    for(auto i : ite->depends)
    {
        _depend += (i + " ");
    }
    label_depend->setText(_depend);
    glay_groupBase->addWidget(label_depend,10,1,1,4);

    glay_groupBase->addWidget(new QLabel("Optdepends:"),11,0);
    QLabel *label_optdepends = new QLabel;
    QString _optdepends;
    for(auto i : ite->optdepends)
    {
        _optdepends += (i + " ");
    }
    label_optdepends->setText(_optdepends);
    glay_groupBase->addWidget(label_optdepends,11,1,1,4);

    glay_groupBase->addWidget(new QLabel("Makedepends:"),12,0);
    QLabel *label_makedepend = new QLabel;
    QString _makedepends;
    for(auto i : ite->makedepends)
    {
        _makedepends += (i + " ");
    }
    label_makedepend->setText(_makedepends);
    glay_groupBase->addWidget(label_makedepend,12,1,1,4);

    glay_groupBase->addWidget(new QLabel("Checkdepends:"),13,0);
    QLabel *label_checkdepend = new QLabel;
    QString _checkdepends;
    for(auto i : ite->checkdepends)
    {
        _checkdepends += (i + " ");
    }
    label_checkdepend->setText(_checkdepends);
    glay_groupBase->addWidget(label_checkdepend,13,1,1,4);

    glay_groupBase->addWidget(new QLabel("Groups:"),14,0);
    QLabel *label_group = new QLabel;
    QString _group;
    for(auto i : ite->groups)
    {
        _group += (i + " ");
    }
    label_group->setText(_group);
    glay_groupBase->addWidget(label_group,14,1,1,4);

    glay_groupBase->addWidget(new QLabel("Pkgdesc:"),15,0);
    QTextBrowser *brower_pkgdesc = new QTextBrowser;
    brower_pkgdesc->setText(ite->pkgdesc);
    glay_groupBase->addWidget(brower_pkgdesc,15,1,3,3);

    mainLay->addWidget(groupBase);


    // OfficialPkg Group
    if(!ite->isAurPkg())
    {QGroupBox *groupOff = new QGroupBox("官方仓库信息");
        QGridLayout *glay_groupOff = new QGridLayout(groupOff);

        glay_groupOff->addWidget(new QLabel("Repo:"),0,0);
        glay_groupOff->addWidget(new QLabel("Filename:"),0,3);
        glay_groupOff->addWidget(new QLabel("Compressed Size:"),1,0);
        glay_groupOff->addWidget(new QLabel("Installed Size:"),1,3);
        glay_groupOff->addWidget(new QLabel("Build Date:"),2,0);
        glay_groupOff->addWidget(new QLabel("Last Update:"),2,3);
        glay_groupOff->addWidget(new QLabel("Flag Date:"),3,0);
        glay_groupOff->addWidget(new QLabel("Packager:"),4,0);

        OfficialPkgItem *offIte = static_cast<OfficialPkgItem*>(ite);

        glay_groupOff->addWidget(new QLabel(offIte->repo),0,1);
        glay_groupOff->addWidget(new QLabel(offIte->filename),0,4);

        glay_groupOff->addWidget(new QLabel(QString::number(offIte->compressed_size)),1,1);
        glay_groupOff->addWidget(new QLabel(QString::number(offIte->installed_size)),1,4);

        glay_groupOff->addWidget(new QLabel(offIte->build_date),2,1);
        glay_groupOff->addWidget(new QLabel(offIte->last_update),2,4);

        glay_groupOff->addWidget(new QLabel(offIte->flag_date),3,1);

        glay_groupOff->addWidget(new QLabel(offIte->packager),4,1);

        mainLay->addWidget(groupOff);
    }


    // Aur Group
    if(ite->isAurPkg())
    {
        QGroupBox *groupAur = new QGroupBox("Aur信息");
        QGridLayout *glay_groupAur = new QGridLayout(groupAur);

        glay_groupAur->addWidget(new QLabel("ID:"),0,0);
        glay_groupAur->addWidget(new QLabel("PackageBase ID:"),0,3);
        glay_groupAur->addWidget(new QLabel("Num Votes:"),1,0);
        glay_groupAur->addWidget(new QLabel("Popularity:"),1,3);
        glay_groupAur->addWidget(new QLabel("Out of Date:"),2,0);
        glay_groupAur->addWidget(new QLabel("First Submited:"),3,0);
        glay_groupAur->addWidget(new QLabel("Last Modified:"),3,3);
        glay_groupAur->addWidget(new QLabel("Url Path:"),4,0);
        glay_groupAur->addWidget(new QLabel("Keywords:"),5,0);

        AurPkgItem *aurIte = static_cast<AurPkgItem*>(ite);

        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->id)),0,1);
        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->packageBaseId)),0,4);

        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->numVotes)),1,1);
        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->popularity)),1,4);

        glay_groupAur->addWidget(new QLabel(aurIte->outOfDate),2,1);

        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->firstSubmitted)),3,1);
        glay_groupAur->addWidget(new QLabel(QString::number(aurIte->lastModified)),3,4);

        glay_groupAur->addWidget(new QLabel(aurIte->urlPath),4,1);

        QLabel *label_keywords = new QLabel;
        QString _keywords;
        for(auto i : aurIte->keywords)
        {
            _keywords += (i + " ");
        }
        label_keywords->setText(_keywords);
        glay_groupAur->addWidget(label_keywords,5,1,1,4);

        mainLay->addWidget(groupAur);
    }

}


