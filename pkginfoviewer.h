#ifndef PKGINFOVIEWER_H
#define PKGINFOVIEWER_H

#include <QMainWindow>

#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QTextBrowser>
#include <QMenu>
#include <QMenuBar>
#include <QAction>

#include "aurpkgitem.h"
#include "officialpkgitem.h"
#include "querythread.h"
#include "querymode.h"
#include "matchmode.h"

class PkgInfoViewer : public QMainWindow
{
    Q_OBJECT
public:
    explicit PkgInfoViewer(PkgItem* item, QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLay;
signals:
    void searchAURInfo(QString target, QueryMode queryMode, MatchMode matchMode);

};

#endif // PKGINFOVIEWER_H
