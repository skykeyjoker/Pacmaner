#include "operationsgui.h"

OperationsGUI::OperationsGUI(QVector<Operation*> ope, QWidget *parent)
    : QDialog(parent),operations(ope)
{
    // Windows
    setWindowFlag(Qt::Dialog);
    setWindowTitle("操作进度");
    setWindowIcon(QIcon(":/icons/progress.png"));
    setFixedSize(QSize(800,600));

    current = 0;

    // Init UI
    mainLay = new QVBoxLayout(this);

    group_operations = new QGroupBox("操作队列");
    QVBoxLayout *lay_groupOperations = new QVBoxLayout(group_operations);
    table = new QTableWidget;
    lay_groupOperations->addWidget(table);

    table->setColumnCount(3);
    table->setHorizontalHeaderLabels(QStringList()<<"状态"<<"操作"<<"包名");
    table->setColumnWidth(0,80);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,120);
    table->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);  //设置为选择单位为行
    table->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择一行
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);  //设置为只读

    for(auto ite : operations)
    {
        table->setRowCount(table->rowCount()+1);

        int currentRow = table->rowCount()-1;
        if(ite->mode == OperationMode::Install)
        {
            QTableWidgetItem *status;
            if(currentRow == 0)
                status = new QTableWidgetItem("完成");
            else
                status = new QTableWidgetItem("等待");

            QTableWidgetItem *mode = new QTableWidgetItem("安装");
            QTableWidgetItem *name = new QTableWidgetItem(ite->pkgname);
            table->setItem(currentRow,0,status);
            table->setItem(currentRow, 1, mode);
            table->setItem(currentRow, 2, name);
        }
        else
        {
            QTableWidgetItem *status;
            if(currentRow == 0)
                status = new QTableWidgetItem("完成");
            else
                status = new QTableWidgetItem("等待");
            QTableWidgetItem *mode = new QTableWidgetItem("卸载");
            QTableWidgetItem *name = new QTableWidgetItem(ite->pkgname);
            table->setItem(currentRow,0,status);
            table->setItem(currentRow, 1, mode);
            table->setItem(currentRow, 2, name);
        }
    }


    group_progress = new QGroupBox("进度");
    QVBoxLayout *lay_groupProgress = new QVBoxLayout(group_progress);
    progressBar = new QProgressBar;
    label = new QLabel;

    progressBar->setRange(0,operations.size());
    progressBar->setValue(0);



    lay_groupProgress->addWidget(progressBar);
    lay_groupProgress->addWidget(label);


    mainLay->addWidget(group_operations);
    mainLay->addWidget(group_progress);

}

void OperationsGUI::updateProgress()
{
    // Update Progress
    operations.remove(0);
    progressBar->setValue(progressBar->value()+1);
    table->item(current,0)->setText("完成");
    current++;
    if(current<table->rowCount())
        label->setText(tr("正在%1 %2 ...").arg(table->item(current,1)->text()).arg(table->item(current,2)->text()));
}
