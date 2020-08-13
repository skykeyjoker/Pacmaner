#ifndef PACMANERMAINGUI_H
#define PACMANERMAINGUI_H

#include <QMainWindow>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QGroupBox>
#include <QGridLayout>
#include <QCheckBox>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QThread>
#include <QAction>
#include <QMenu>
#include <QMetaType>
#include <QModelIndex>
#include <QDialog>
#include <QProcess>
#include <QPixmap>

#include "aurpkgitem.h"
#include "officialpkgitem.h"
#include "querymode.h"
#include "matchmode.h"
#include "querythread.h"
#include "pkginfoviewer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PacmanerMainGui; }
QT_END_NAMESPACE


class PacmanerMainGui : public QMainWindow
{
    Q_OBJECT

public:
    PacmanerMainGui(QWidget *parent = nullptr);
    ~PacmanerMainGui();


signals:
    void querySignal(QString target, QueryMode queryMode, MatchMode matchMode);
    void searchBtnClicked(QString target, QueryMode queryMode, MatchMode matchMode, QTableWidget* table);

public slots:
    void querySlot(QString target, QueryMode queryMode, MatchMode matchMode, QTableWidget* table);
    void baseTableMenu(QPoint);
    void showPkgInfoViewer(PkgItem* item);
    void aurinfoSearchFinished(QVector<PkgItem*> result);


private:
    Ui::PacmanerMainGui *ui;

    QVBoxLayout *vlay_baseTab;
    QVBoxLayout *vlay_aurTab;
    QVBoxLayout *vlay_localTab;

    QTableWidget *table_baseTab;

    QThread *mainQueryThread;

    QVector<PkgItem*> pkgQueryResults;
};
#endif // PACMANERMAINGUI_H
