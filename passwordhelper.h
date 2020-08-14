#ifndef PASSWORDHELPER_H
#define PASSWORDHELPER_H

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QDebug>
#include <QMessageBox>

class PasswordHelper : public QDialog
{
    Q_OBJECT
public:
    explicit PasswordHelper(QWidget *parent = nullptr);
    QVBoxLayout *mainlay;

private:
    QString password;
    QByteArray buf;
signals:
    void passwordAccessible(const QString &_password);

};

#endif // PASSWORDHELPER_H
