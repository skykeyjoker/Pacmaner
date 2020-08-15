#ifndef OPERATIONSGUI_H
#define OPERATIONSGUI_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QProgressBar>
#include <QHeaderView>

#include "operation.h"
#include "OperationMode.h"

class OperationsGUI : public QDialog
{
    Q_OBJECT
public:
    explicit OperationsGUI(QVector<Operation*>ope, QWidget *parent = nullptr);


private:
    QVBoxLayout *mainLay;
    QProgressBar *progressBar;
    QTableWidget *table;
    QVector<Operation*> operations;
    QGroupBox *group_operations;
    QGroupBox *group_progress;
    QLabel *label;
    int current = 0;

signals:

public slots:
    void updateProgress();

};

#endif // OPERATIONSGUI_H
