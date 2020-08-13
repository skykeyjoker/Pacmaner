#include "pacmanermaingui.h"
#include "ui_pacmanermaingui.h"

PacmanerMainGui::PacmanerMainGui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PacmanerMainGui)
{

    qRegisterMetaType<PkgItem*>("QVector<PkgItem*>");

    ui->setupUi(this);
    setWindowIcon(QIcon(QIcon(":/icons/icon.png")));

    // Thread
    mainQueryThread = new QThread;
    mainQueryThread->start();

    // Signals and Slots
    void (PacmanerMainGui::*pSearchBtnClicked)(QString target, QueryMode queryMode, MatchMode matchMode, QTableWidget* table) = &PacmanerMainGui::searchBtnClicked;
    void (PacmanerMainGui::*pQuerySlot)(QString target, QueryMode queryMode, MatchMode matchMode, QTableWidget* table) = &PacmanerMainGui::querySlot;
    connect(this, pSearchBtnClicked,
            this, pQuerySlot);


    // Init UI
    vlay_baseTab = new QVBoxLayout(ui->tab_base);
    vlay_aurTab = new QVBoxLayout(ui->tab_aur);
    vlay_localTab = new QVBoxLayout(ui->tab_local);


    // Base Tab
    table_baseTab = new QTableWidget;
    QGroupBox *group_baseTab = new QGroupBox("检索");
    QGridLayout *glay_groupBaseTab = new QGridLayout(group_baseTab);

    QLineEdit *le_baseTab = new QLineEdit;
    QComboBox *combo_baseTab = new QComboBox;
    QCheckBox *check_baseTab = new QCheckBox("仅检索包名");
    QPushButton *btn_searchBaseTab = new QPushButton("检索");

    glay_groupBaseTab->addWidget(new QLabel("包名："),0,0);
    glay_groupBaseTab->addWidget(le_baseTab,0,1);
    glay_groupBaseTab->addWidget(combo_baseTab,1,1);
    glay_groupBaseTab->addWidget(check_baseTab,1,2);
    glay_groupBaseTab->addWidget(btn_searchBaseTab,0,3,2,2);


    vlay_baseTab->addWidget(group_baseTab);
    vlay_baseTab->addWidget(table_baseTab);

    combo_baseTab->addItem("仅检索官方仓库");
    combo_baseTab->addItem("仅检索AUR");
    combo_baseTab->addItem("同时检索官方仓库及AUR");


    table_baseTab->setColumnCount(5);
    table_baseTab->setHorizontalHeaderLabels(QStringList()<<"状态"<<"AUR"<<"包名"<<"版本"<<"描述");
    table_baseTab->setColumnWidth(0,80);
    table_baseTab->setColumnWidth(1,80);
    table_baseTab->setColumnWidth(2,80);
    table_baseTab->setColumnWidth(3,180);
    table_baseTab->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);

    table_baseTab->setContextMenuPolicy(Qt::CustomContextMenu);  //允许自定义右键菜单
    void (QTableWidget::*pCustomContextMenuRequested)(const QPoint &) = &QTableWidget::customContextMenuRequested;
    void (PacmanerMainGui::*pBaseTableMenu)(QPoint) = &PacmanerMainGui::baseTableMenu;
    connect(table_baseTab, pCustomContextMenuRequested, this, pBaseTableMenu);

    connect(table_baseTab, &QTableWidget::doubleClicked, [=](const QModelIndex &index){
        qDebug()<<"Double Clicked! Row:"<<index.row();
        showPkgInfoViewer(pkgQueryResults.at(index.row()));
    });

    table_baseTab->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置为选择单位为行
    table_baseTab->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择一行
    table_baseTab->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置为只读

    connect(btn_searchBaseTab, &QPushButton::clicked,[=](){
        if(le_baseTab->text().isEmpty())
            QMessageBox::critical(this,"查询失败","请输入包名！");
        else
        {
            if(check_baseTab->isChecked() == false)
                emit searchBtnClicked(le_baseTab->text(),(QueryMode)combo_baseTab->currentIndex(),MatchMode::NameAndDesc,table_baseTab);
            else
                emit searchBtnClicked(le_baseTab->text(),(QueryMode)combo_baseTab->currentIndex(),MatchMode::OnlyName,table_baseTab);
        }
    });

}

PacmanerMainGui::~PacmanerMainGui()
{
    connect(mainQueryThread, &QThread::finished, mainQueryThread, &QThread::deleteLater);
    mainQueryThread->quit();

    delete ui;
}

void PacmanerMainGui::querySlot(QString target, QueryMode queryMode, MatchMode matchMode, QTableWidget *table)
{
    qDebug()<<"querySlot";

    for(int i = table->rowCount()-1; i>=0; --i)
        table->removeRow(i);

    pkgQueryResults.clear();

    // Sub Thread
    QueryThread *subQueryThread = new QueryThread;
    subQueryThread->moveToThread(mainQueryThread);

    void (PacmanerMainGui::*pQuerySignal)(QString target, QueryMode queryMode, MatchMode matchMode) = &PacmanerMainGui::querySignal;
    void (QueryThread::*pStartQuery)(QString target, QueryMode queryMode, MatchMode matchMode) = &QueryThread::startQuery;
    connect(this,pQuerySignal,subQueryThread,pStartQuery,Qt::QueuedConnection);

    // Result
    connect(subQueryThread, &QueryThread::queryFinished,[=](QVector<PkgItem*> result){
        qDebug()<<"result size:"<<result.size();

        pkgQueryResults = result;

        for(auto ite : result)
        {
            //table_baseTab->setHorizontalHeaderLabels(QStringList()<<"状态"<<"AUR"<<"包名"<<"版本"<<"描述");

            table->setRowCount(table->rowCount()+1);

            QTableWidgetItem *pkgname = new QTableWidgetItem;
            pkgname->setText(ite->pkgname);

            QTableWidgetItem *pkgdesc = new QTableWidgetItem;
            pkgdesc->setText(ite->pkgdesc);

            QTableWidgetItem *isaur = new QTableWidgetItem;
            if(ite->isAurPkg())
                isaur->setIcon(QIcon(":/icons/duihao.png"));

            QTableWidgetItem *pkgver = new QTableWidgetItem;
            pkgver->setText(tr("%1-%2").arg(ite->pkgver).arg(ite->pkgrel));

            table->setItem(table->rowCount()-1,1,isaur);
            table->setItem(table->rowCount()-1,2,pkgname);
            table->setItem(table->rowCount()-1,3,pkgver);
            table->setItem(table->rowCount()-1,4,pkgdesc);
        }

    });
    connect(subQueryThread, &QueryThread::queryFinished,subQueryThread,&QueryThread::deleteLater);

    //subQueryThread->startQuery("pacman",QueryMode::OnlyOfficial,MatchMode::NameAndDesc);
    emit querySignal(target,queryMode,matchMode);
}

void PacmanerMainGui::baseTableMenu(QPoint pos)
{
    int x = pos.x ();
    int y = pos.y ();
    QModelIndex index = table_baseTab->indexAt (QPoint(x,y));
    int row = index.row ();
    qDebug()<<row;

    if(row != -1)
    {QMenu *menu = new QMenu(table_baseTab);

        QAction *act_info = new QAction("详情",table_baseTab);
        QAction *act_ins = new QAction("安装",table_baseTab);
        QAction *act_uins = new QAction("卸载",table_baseTab);
        QAction *act_apply = new QAction("应用该更改",table_baseTab);
        QAction *act_cancel = new QAction("取消该更改",table_baseTab);

        menu->addAction(act_info);
        menu->addAction(act_ins);
        menu->addAction(act_uins);
        menu->addAction(act_apply);
        menu->addAction(act_cancel);

        menu->move (cursor().pos());
        menu->show ();





        connect(act_info, &QAction::triggered,[=](){
            showPkgInfoViewer(pkgQueryResults.at(row));
        });
    }
}

void PacmanerMainGui::showPkgInfoViewer(PkgItem *item)
{
    if(item->isAurPkg())
    {
        QDialog *dialog = new QDialog(this);
        dialog->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(new QLabel(tr("正在检索 %1 包AUR信息，请稍候。").arg(item->pkgname)));
        dialog->setLayout(lay);

        dialog->show();

        QNetworkAccessManager *manager =  new QNetworkAccessManager(this);

        connect(manager, &QNetworkAccessManager::finished,[=](QNetworkReply* reply)
        {
            qDebug()<<"Query Finished";

            QNetworkReply::NetworkError err = reply->error();
            if(err != QNetworkReply::NoError)
            {
                qDebug() << "Failed: " << reply->errorString();
            }
            else
            {
                QByteArray buf = reply->readAll();

                // Convert to JSON
                QJsonParseError err;
                QJsonDocument docment = QJsonDocument::fromJson(buf, &err);

                if(err.error != QJsonParseError::NoError)
                {
                    qDebug()<<"Can't import json data.";
                    exit(0);
                }
                else
                {
                    // "results" Array
                    QJsonArray resultsArr = docment.object().value("results").toArray();
                    for(auto item : resultsArr)
                    {
                        QJsonObject itemObj = item.toObject();

                        qlonglong id = itemObj.value("ID").toInt();
                        qlonglong pkgbaseid = itemObj.value("PackageBaseID").toInt();
                        QString pkgname = itemObj.value("Name").toString();
                        QString pkgbase = "";
                        QString arch = "";
                        QString pkgver = itemObj.value("Version").toString().section('-',0,0);
                        int pkgrel = itemObj.value("Version").toString().section('-',1,1).toInt();
                        int epoch = 1;
                        QString pkgdesc = itemObj.value("Description").toString();
                        QString url = itemObj.value("URL").toString();
                        int numvotes = itemObj.value("NumVotes").toInt();
                        double popularity = itemObj.value("Popularity").toDouble();
                        QString outofdate = itemObj.value("OutOfDate").toString();
                        qlonglong firstsubmitted = itemObj.value("FirstSubmitted").toInt();
                        qlonglong lastmodified = itemObj.value("LastModified").toInt();
                        QString urlpath = itemObj.value("URLPath").toString();

                        QVector<QString> maintainers;
                        QJsonValue maintainersValue = itemObj.value("maintainers");
                        if(maintainersValue.isArray())
                        {
                            QJsonArray maintainersArr = maintainersValue.toArray();

                            for(auto maintainerItem : maintainersArr)
                            {
                                maintainers.push_back(maintainerItem.toString());
                            }
                        }
                        else
                            maintainers.push_back(maintainersValue.toString());

                        QString packager = itemObj.value("packager").toString();

                        QVector<QString> licenses;
                        QJsonValue licensesValue = itemObj.value("License");
                        if(licensesValue.isArray())
                        {
                            QJsonArray licensesArr = licensesValue.toArray();

                            for(auto licenseItem : licensesArr)
                            {
                                licenses.push_back(licenseItem.toString());
                            }
                        }
                        else
                            licenses.push_back(licensesValue.toString());

                        QVector<QString> conflicts;
                        QJsonValue conflictsValue = itemObj.value("Conflicts");
                        if(conflictsValue.isArray())
                        {
                            QJsonArray conflictsArr = conflictsValue.toArray();

                            for(auto conflictItem : conflictsArr)
                            {
                                conflicts.push_back(conflictItem.toString());
                            }
                        }
                        else
                            conflicts.push_back(conflictsValue.toString());

                        QVector<QString> provides;
                        QJsonValue providesValue = itemObj.value("Provides");
                        if(providesValue.isArray())
                        {
                            QJsonArray providesArr = providesValue.toArray();

                            for(auto provideItem : providesArr)
                            {
                                provides.push_back(provideItem.toString());
                            }
                        }
                        else
                            provides.push_back(providesValue.toString());

                        QVector<QString> replaces;
                        QJsonValue replacesValue = itemObj.value("Replaces");
                        if(replacesValue.isArray())
                        {
                            QJsonArray replacesArr = replacesValue.toArray();

                            for(auto replaceItem : replacesArr)
                            {
                                replaces.push_back(replaceItem.toString());
                            }
                        }
                        else
                            replaces.push_back(replacesValue.toString());

                        QVector<QString> depends;
                        QJsonValue dependsValue = itemObj.value("Depends");
                        if(dependsValue.isArray())
                        {
                            QJsonArray dependsArr = dependsValue.toArray();

                            for(auto dependItem : dependsArr)
                            {
                                depends.push_back(dependItem.toString());
                            }
                        }
                        else
                            depends.push_back(dependsValue.toString());

                        QVector<QString> optdepends;
                        QJsonValue optdependsValue = itemObj.value("OptDepends");
                        if(optdependsValue.isArray())
                        {
                            QJsonArray optdependsArr = optdependsValue.toArray();

                            for(auto optdependItem : optdependsArr)
                            {
                                optdepends.push_back(optdependItem.toString());
                            }
                        }
                        else
                            optdepends.push_back(optdependsValue.toString());

                        QVector<QString> makedepends;
                        QJsonValue makedependsValue = itemObj.value("MakeDepends");
                        if(makedependsValue.isArray())
                        {
                            QJsonArray makedependsArr = makedependsValue.toArray();

                            for(auto makedependItem : makedependsArr)
                            {
                                makedepends.push_back(makedependItem.toString());
                            }
                        }
                        else
                            makedepends.push_back(makedependsValue.toString());

                        QVector<QString> checkdepends;
                        QJsonValue checkdependsValue = itemObj.value("CheckDepends");
                        if(checkdependsValue.isArray())
                        {
                            QJsonArray checkdependsArr = checkdependsValue.toArray();

                            for(auto checkdependItem : checkdependsArr)
                            {
                                checkdepends.push_back(checkdependItem.toString());
                            }
                        }
                        else
                            checkdepends.push_back(checkdependsValue.toString());

                        QVector<QString> groups;
                        QJsonValue groupsValue = itemObj.value("Groups");
                        if(groupsValue.isArray())
                        {
                            QJsonArray groupsArr = groupsValue.toArray();

                            for(auto groupItem : groupsArr)
                            {
                                groups.push_back(groupItem.toString());
                            }
                        }
                        else
                            groups.push_back(groupsValue.toString());

                        QVector<QString> keywords;
                        QJsonValue keywordsValue = itemObj.value("Keywords");
                        if(keywordsValue.isArray())
                        {
                            QJsonArray keywordsArr = keywordsValue.toArray();

                            for(auto keywordItem : keywordsArr)
                            {
                                keywords.push_back(keywordItem.toString());
                            }
                        }
                        else
                            keywords.push_back(keywordsValue.toString());


                        AurPkgItem *tmpItem
                                = new AurPkgItem(pkgname,pkgbase,arch,
                                                 pkgver,pkgrel,epoch,
                                                 pkgdesc,url,maintainers,
                                                 licenses,conflicts,provides,
                                                 replaces,depends,optdepends,
                                                 makedepends,checkdepends,groups,
                                                 id,pkgbaseid,numvotes,
                                                 popularity,outofdate,firstsubmitted,
                                                 lastmodified,urlpath,keywords);

                        tmpItem->printInfo();

                        dialog->close();

                        PkgInfoViewer *viewer = new PkgInfoViewer(tmpItem, this);
                        viewer->show();
                    }

                }
            }
        });
        QNetworkRequest request;
        request.setUrl(tr("https://aur.archlinux.org/rpc/?v=5&type=info&arg[]=%1").arg(item->pkgname));

        manager->get(request);
    }
    else
    {
        PkgInfoViewer *viewer = new PkgInfoViewer(item, this);
        viewer->show();
    }
}

