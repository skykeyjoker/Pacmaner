#include "pacmanermaingui.h"
#include "ui_pacmanermaingui.h"

PacmanerMainGui::PacmanerMainGui(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PacmanerMainGui)
{

    qRegisterMetaType<PkgItem*>("QVector<PkgItem*>");
    qRegisterMetaType<Operation*>("QVector<Operation*>");
    qRegisterMetaType<Operation*>("Operation*");

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
    table_baseTab->setColumnWidth(0,120);
    table_baseTab->setColumnWidth(1,80);
    table_baseTab->setColumnWidth(2,180);
    table_baseTab->setColumnWidth(3,160);
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
    connect(subQueryThread, &QueryThread::queryFinished, [=](QVector<PkgItem*> result){
        qDebug()<<"result size:"<<result.size();

        pkgQueryResults = result;

        for(auto ite : result)
        {
            //table_baseTab->setHorizontalHeaderLabels(QStringList()<<"状态"<<"AUR"<<"包名"<<"版本"<<"描述");

            table->setRowCount(table->rowCount()+1);
            int currentRow = table->rowCount()-1;


            QTableWidgetItem *pkgname = new QTableWidgetItem;
            pkgname->setText(ite->pkgname);

            QTableWidgetItem *pkgdesc = new QTableWidgetItem;
            pkgdesc->setText(ite->pkgdesc);


            QTableWidgetItem *isaur = new QTableWidgetItem;
            if(ite->isAurPkg())
            {
                isaur->setIcon(QIcon(":/icons/duihao.png"));
                isaur->setText("AUR");
            }

            QTableWidgetItem *pkgver = new QTableWidgetItem;
            pkgver->setText(tr("%1-%2").arg(ite->pkgver).arg(ite->pkgrel));

            // Package Status

            QProcess *pacman = new QProcess;
            connect(pacman, &QProcess::readyReadStandardError, [=](){
                QTableWidgetItem *item_status = new QTableWidgetItem;

                item_status->setIcon(QIcon(":/icons/notinstalled.png"));
                item_status->setText("未安装");
                table->setItem(currentRow,0,item_status);
            });
            connect(pacman, &QProcess::readyReadStandardOutput, [=](){
                QTableWidgetItem *item_status = new QTableWidgetItem;

                item_status->setText("已安装");
                item_status->setIcon(QIcon(":/icons/alreadyinstalled.png"));
                table->setItem(currentRow,0,item_status);
            });
            pacman->start(tr("pacman -Q %1").arg(ite->pkgname));

            // Set Items
            table->setItem(currentRow,1,isaur);
            table->setItem(currentRow,2,pkgname);
            table->setItem(currentRow,3,pkgver);
            table->setItem(currentRow,4,pkgdesc);
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

        if(table_baseTab->item(row,0)->text() == "未安装")
        {
            act_uins->setEnabled(false);
            act_apply->setEnabled(false);
            act_cancel->setEnabled(false);
        }

        if(table_baseTab->item(row,0)->text() == "已安装")
        {
            act_ins->setEnabled(false);
            act_apply->setEnabled(false);
            act_cancel->setEnabled(false);
        }

        if(table_baseTab->item(row,0)->text() == "准备安装" || table_baseTab->item(row,0)->text() == "准备卸载")
        {
            act_ins->setEnabled(false);
            act_uins->setEnabled(false);
        }

        // TODO: 连接菜单信号槽
        connect(act_ins, &QAction::triggered, [=](){
            Operation *ope;
            if(table_baseTab->item(row,1)->text() == "AUR")
                ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Install,true);
            else
                ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Install,false);

            //压入操作队列
            operations.push_back(ope);
            ins_operations.push_back(ope);

            act_apply->setEnabled(true);
            act_cancel->setEnabled(true);

            table_baseTab->item(row,0)->setIcon(QIcon(":/icons/install.png"));
            table_baseTab->item(row,0)->setText("准备安装");
        });
        connect(act_uins, &QAction::triggered, [=](){
            Operation *ope;
            if(table_baseTab->item(row,1)->text() == "AUR")
                ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Uinstall,true);
            else
                ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Uinstall,false);

            //压入操作队列
            operations.push_back(ope);
            uni_operations.push_back(ope);

            act_apply->setEnabled(true);
            act_cancel->setEnabled(true);
            table_baseTab->item(row,0)->setIcon(QIcon(":/icons/remove.png"));
            table_baseTab->item(row,0)->setText("准备卸载");
        });


        connect(act_apply,&QAction::triggered,[=](){
            int ret = QMessageBox::question(this,"请求确认","您确认要应用该操作吗？");
            if(ret == QMessageBox::Yes)
            {

                PasswordHelper *passhelper = new PasswordHelper;

                connect(passhelper, &PasswordHelper::passwordAccessible, [=](const QString &pass){
                    password = pass;
                });

                passhelper->exec();

                if(password.isEmpty())
                {
                    QMessageBox::critical(this, "身份验证失败", "未通过管理员身份验证，停止操作。");
                    return;
                }
                else
                {
                    tmp_operation.clear();

                    Operation *ope;
                    if(table_baseTab->item(row,0)->text() == "准备安装")
                    {
                        if(table_baseTab->item(row,1)->text() == "AUR")
                            ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Install,true);
                        else
                            ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Install,false);
                    }
                    if(table_baseTab->item(row,0)->text() == "准备卸载")
                    {
                        if(table_baseTab->item(row,1)->text() == "AUR")
                            ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Uinstall,true);
                        else
                            ope = new Operation(table_baseTab->item(row,2)->text(),OperationMode::Uinstall,false);
                    }
                    tmp_operation.push_back(ope);


                    QThread *mainOperatorThread = new QThread;
                    OperatorThread *subOperatorThread = new OperatorThread;
                    mainOperatorThread->start();
                    subOperatorThread->moveToThread(mainOperatorThread);

                    void (PacmanerMainGui::*pApplyOperations)(const QVector<Operation*>&, const QString&) = &PacmanerMainGui::applyOperations;
                    void (OperatorThread::*pStartOperator)(const QVector<Operation*>&, const QString&) = &OperatorThread::startOperator;
                    connect(this, pApplyOperations, subOperatorThread, pStartOperator);
                    connect(subOperatorThread, &OperatorThread::operationFailed, [=](QString pkgname, QByteArray err){
                        QMessageBox::critical(this, "操作失败",tr("应用操作失败！\n%1:\n%2").arg(pkgname).arg(QString::fromLocal8Bit(err)));
                        return;
                    });
                    connect(subOperatorThread, &OperatorThread::operationFinished, [=](Operation* o){
                        // 从总列表中移除该操
                        for(int i = 0; i<operations.size(); ++i)
                        {
                            if(operations.at(i)->pkgname == o->pkgname)
                            {
                                qDebug()<<"operation found";
                                operations.remove(i);
                            }
                        }
                        for(int i = 0; i<ins_operations.size(); ++i)
                        {
                            if(ins_operations.at(i)->pkgname == o->pkgname)
                            {
                                qDebug()<<"ins_operations found";
                                ins_operations.remove(i);
                            }
                        }
                        for(int i = 0; i<uni_operations.size(); ++i)
                        {
                            if(uni_operations.at(i)->pkgname == o->pkgname)
                            {
                                qDebug()<<"uni_operations found";
                                uni_operations.remove(i);
                            }
                        }

                    });
                    connect(subOperatorThread, &OperatorThread::allOperationFinished, subOperatorThread, &OperatorThread::deleteLater);
                    connect(subOperatorThread,&OperatorThread::destroyed, mainOperatorThread, &QThread::quit);
                    connect(mainOperatorThread,&QThread::finished, mainOperatorThread, &QThread::deleteLater);


                    emit applyOperations(tmp_operation,password);

                    // 更改软件包状态
                    if(table_baseTab->item(row,0)->text() == "准备安装")
                    {
                        table_baseTab->item(row,0)->setText("已安装");
                        table_baseTab->item(row,0)->setIcon(QIcon(":/icons/alreadyinstalled.png"));

                    }

                    if(table_baseTab->item(row,0)->text() == "准备卸载")
                    {
                        table_baseTab->item(row,0)->setText("未安装");
                        table_baseTab->item(row,0)->setIcon(QIcon(":/icons/notinstalled.png"));
                    }
                }
            }
        });
        connect(act_cancel,&QAction::triggered,[=](){
            int ret = QMessageBox::question(this,"请求确认","您确认要取消该操作吗？");
            if(ret == QMessageBox::Yes)
            {
                if(table_baseTab->item(row,0)->text() == "准备安装")
                {
                    table_baseTab->item(row,0)->setText("未安装");
                    table_baseTab->item(row,0)->setIcon(QIcon(":/icons/notinstalled.png"));
                }

                if(table_baseTab->item(row,0)->text() == "准备卸载")
                {
                    table_baseTab->item(row,0)->setText("已安装");
                    table_baseTab->item(row,0)->setIcon(QIcon(":/icons/alreadyinstalled.png"));
                }

                // 从列表中移除当前操作
                for(int i = 0; i<operations.size(); ++i)
                {
                    if(operations.at(i)->pkgname == table_baseTab->item(row,2)->text())
                        operations.remove(i);
                }


                for(int i = 0; i<ins_operations.size(); ++i)
                {
                    if(ins_operations.at(i)->pkgname == table_baseTab->item(row,2)->text())
                        ins_operations.remove(i);
                }


                for(int i = 0; i<uni_operations.size(); ++i)
                {
                    if(uni_operations.at(i)->pkgname == table_baseTab->item(row,2)->text())
                        uni_operations.remove(i);
                }

            }
        });


        connect(act_info, &QAction::triggered,[=](){
            showPkgInfoViewer(pkgQueryResults.at(row));
        });
    }
}

void PacmanerMainGui::showPkgInfoViewer(PkgItem *item)
{
    if(item->isAurPkg())
    {
        // Loading Dialog
        QDialog *dialog = new QDialog(this);
        dialog->setWindowFlag(Qt::FramelessWindowHint);
        QVBoxLayout *lay = new QVBoxLayout;
        lay->addWidget(new QLabel(tr("正在检索 %1 包AUR信息，请稍候。").arg(item->pkgname)));
        dialog->setLayout(lay);


        QueryThread *subQueryThread = new QueryThread;
        subQueryThread->moveToThread(mainQueryThread);

        void (PacmanerMainGui::*pQuerySignal)(QString target, QueryMode queryMode, MatchMode matchMode) = &PacmanerMainGui::querySignal;
        void (QueryThread::*pStartQuery)(QString target, QueryMode queryMode, MatchMode matchMode) = &QueryThread::startQuery;
        connect(this,pQuerySignal,subQueryThread,pStartQuery,Qt::QueuedConnection);

        // Result
        void (QueryThread::*pQueryFinished)(QVector<PkgItem*>) = &QueryThread::queryFinished;
        void (PacmanerMainGui::*pAurinfoSearchFinished)(QVector<PkgItem*>) = &PacmanerMainGui::aurinfoSearchFinished;
        connect(subQueryThread, pQueryFinished, this, pAurinfoSearchFinished);
        connect(subQueryThread, &QueryThread::queryFinished,[=](){
            dialog->close();
        });
        // Delete Subthread
        connect(subQueryThread, &QueryThread::queryFinished,subQueryThread,&QueryThread::deleteLater);


        emit querySignal(item->pkgname,QueryMode::AurInfo,MatchMode::OnlyName);

        // 使用exec方式，以模态对话框的形式展示，禁止在加载期间用户对主界面进行操作。
        dialog->exec();

    }
    else
    {
        PkgInfoViewer *viewer = new PkgInfoViewer(item, this);
        viewer->show();
    }
}

void PacmanerMainGui::aurinfoSearchFinished(QVector<PkgItem*> result)
{
    PkgInfoViewer *viewer = new PkgInfoViewer(result.at(0), this);
    viewer->show();
}



void PacmanerMainGui::on_act_applyAll_triggered()
{
    if(!operations.isEmpty())
    {
        int ret = QMessageBox::question(this, "请求确认",
                                        tr("您确定要应用所有操作吗？\n本次将安装%1个软件包，卸载%2个软件包").arg(ins_operations.size()).arg(uni_operations.size()));

        if(ret == QMessageBox::Yes)
        {
            PasswordHelper *passhelper = new PasswordHelper;

            connect(passhelper, &PasswordHelper::passwordAccessible, [=](const QString &pass){
                password = pass;
            });

            passhelper->exec();

            if(password.isEmpty())
            {
                QMessageBox::critical(this, "身份验证失败", "未通过管理员身份验证，停止操作。");
                return;
            }
            else
            {
                // TODO 实时操作界面
                OperationsGUI *progressWindow = new OperationsGUI(operations,this);

                qDebug()<<"Apply All "<<operations.size();
                QThread *mainOperatorThread = new QThread;
                OperatorThread *subOperatorThread = new OperatorThread;
                mainOperatorThread->start();
                subOperatorThread->moveToThread(mainOperatorThread);

                void (PacmanerMainGui::*pApplyOperations)(const QVector<Operation*>&, const QString&) = &PacmanerMainGui::applyOperations;
                void (OperatorThread::*pStartOperator)(const QVector<Operation*>&, const QString&) = &OperatorThread::startOperator;
                connect(this, pApplyOperations, subOperatorThread, pStartOperator);
                connect(subOperatorThread, &OperatorThread::operationFailed, [=](const QString &pkgname, const QByteArray &err){
                    QMessageBox::critical(this, "操作失败",tr("应用操作失败！\n%1:\n%2").arg(pkgname).arg(QString::fromLocal8Bit(err)));
                    return;
                });
                connect(subOperatorThread, &OperatorThread::operationFinished,[=](Operation *ope){
                    // 更新表格包状态
                    for(int i = 0; i<table_baseTab->rowCount(); ++i)
                    {
                        if(table_baseTab->item(i,2)->text()==ope->pkgname)
                        {
                            if(table_baseTab->item(i,0)->text() == "准备安装")
                            {
                                table_baseTab->item(i,0)->setText("已安装");
                                table_baseTab->item(i,0)->setIcon(QIcon(":/icons/alreadyinstalled.png"));

                            }

                            if(table_baseTab->item(i,0)->text() == "准备卸载")
                            {
                                table_baseTab->item(i,0)->setText("未安装");
                                table_baseTab->item(i,0)->setIcon(QIcon(":/icons/notinstalled.png"));
                            }
                            break;
                        }
                    }

                    // 从容器中移除当前操作
                    for(int i = 0; i<operations.size(); ++i)
                    {
                        if(operations.at(i)->pkgname == ope->pkgname)
                        {
                            qDebug()<<"operation found";
                            operations.remove(i);
                        }
                        break;
                    }
                    for(int i = 0; i<ins_operations.size(); ++i)
                    {
                        if(ins_operations.at(i)->pkgname == ope->pkgname)
                        {
                            qDebug()<<"ins_operations found";
                            ins_operations.remove(i);
                        }
                        break;
                    }
                    for(int i = 0; i<uni_operations.size(); ++i)
                    {
                        if(uni_operations.at(i)->pkgname == ope->pkgname)
                        {
                            qDebug()<<"uni_operations found";
                            uni_operations.remove(i);
                        }
                        break;
                    }


                    progressWindow->updateProgress();
                });
                connect(subOperatorThread, &OperatorThread::allOperationFinished,[=](){
                    qDebug()<<"operations.size: "<<operations.size();
                    operations.clear();
                    ins_operations.clear();
                    uni_operations.clear();
                });
                connect(subOperatorThread, &OperatorThread::allOperationFinished, subOperatorThread, &OperatorThread::deleteLater);
                connect(subOperatorThread,&OperatorThread::destroyed, mainOperatorThread, &QThread::quit);
                connect(mainOperatorThread,&QThread::finished, mainOperatorThread, &QThread::deleteLater);


                emit applyOperations(operations,password);
                qDebug()<<"applyOperations emit";

                progressWindow->exec();
            }
        }
        else
            return;
    }
    else
    {
        QMessageBox::critical(this,"错误", "当前操作队列为空！");
    }
}

// 清除所有操作
void PacmanerMainGui::on_act_cancelAll_triggered()
{
    int ret = QMessageBox::question(this,"请求确认","您确定要取消所有操作吗？");

    if(ret == QMessageBox::Yes)
    {
        // 清空容器
        operations.clear();
        ins_operations.clear();
        uni_operations.clear();

        // 还原table图标
        for(int i = 0; i<table_baseTab->rowCount(); ++i)
        {
            if(table_baseTab->item(i,0)->text() == "准备安装")
            {
                table_baseTab->item(i,0)->setText("未安装");
                table_baseTab->item(i,0)->setIcon(QIcon(":/icons/notinstalled.png"));
            }

            if(table_baseTab->item(i,0)->text() == "准备卸载")
            {
                table_baseTab->item(i,0)->setText("已安装");
                table_baseTab->item(i,0)->setIcon(QIcon(":/icons/alreadyinstalled.png"));
            }
        }
    }
}

void PacmanerMainGui::on_act_about_triggered()
{

}

void PacmanerMainGui::on_act_exit_triggered()
{
    exit(0);
}
