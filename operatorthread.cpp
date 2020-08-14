#include "operatorthread.h"
#include <QDebug>

OperatorThread::OperatorThread(QObject *parent) : QObject(parent)
{

}

void OperatorThread::startOperator(const QVector<Operation *> &_operations, const QString &_password)
{
    qDebug()<<"Start Operator.";
    QProcess *echo = new QProcess;
    QProcess *pacman = new QProcess;

    echo->setStandardOutputProcess(pacman);
    pacman->setProcessChannelMode(QProcess::ForwardedChannels);

    for(auto ite:_operations)
    {
        connect(pacman, &QProcess::readyReadStandardError, [=](){
            QByteArray err = pacman->readAllStandardError();
            emit operationFailed(ite->pkgname, err);
        });

        if(ite->mode == OperationMode::Install)
        {
            qDebug()<<"Install: "<<ite->pkgname;
            echo->start(tr("echo %1").arg(_password));
            pacman->start(tr("sudo -S pacman -S %1 --noconfirm --noprogressbar -q").arg(ite->pkgname));
            pacman->waitForFinished();

            qDebug()<<"Installed: "<<ite->pkgname;
            emit operationFinished(ite);
        }

        if(ite->mode == OperationMode::Uinstall)
        {
            qDebug()<<"Uninstall: "<<ite->pkgname;
            echo->start(tr("echo %1").arg(_password));
            pacman->start(tr("sudo -S pacman -R %1 --noconfirm --noprogressbar").arg(ite->pkgname));
            pacman->waitForFinished();
            qDebug()<<"Uninstalled: "<<ite->pkgname;
            emit operationFinished(ite);
        }
    }

    emit allOperationFinished();
    qDebug()<<"All operation finished";
}
