#include "passwordhelper.h"

PasswordHelper::PasswordHelper(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("管理员权限获取");
    setWindowFlag(Qt::Dialog);
    setFixedSize(QSize(550,125));

    mainlay = new QVBoxLayout(this);

    QLabel *info = new QLabel;
    info->setText("安装或卸载包需要管理员(ROOT)权限，请您输入运行帐号的密码：");

    QHBoxLayout *hlay_pass = new QHBoxLayout;
    QLineEdit *le_pass = new QLineEdit;
    QPushButton *btn = new QPushButton("确定");
    hlay_pass->addWidget(le_pass);
    hlay_pass->addWidget(btn);

    le_pass->setEchoMode(QLineEdit::Password);

    mainlay->addWidget(info);
    mainlay->addLayout(hlay_pass);

    connect(btn, &QPushButton::clicked, [=](){
        if(le_pass->text().isEmpty())
        {
            QMessageBox::critical(this,"密码为空","请输入密码!");
        }
        else
        {
            buf.clear();
            password = le_pass->text();

            QProcess *echo = new QProcess;
            QProcess *pacman = new QProcess;

            echo->setStandardOutputProcess(pacman);
            connect(pacman, &QProcess::readyReadStandardOutput, [=](){
                buf = pacman->readAllStandardOutput();
            });

            echo->start(tr("echo %1").arg(le_pass->text()));
            pacman->start("sudo -S pacman -h");
            pacman->setProcessChannelMode(QProcess::ForwardedChannels);
            pacman->waitForFinished();

            if(buf.isEmpty())
            {
                QMessageBox::critical(this,"验证失败","未能通过管理员身份验证，请重新输入并检查密码！");
                le_pass->clear();
            }
            else
            {
                emit passwordAccessible(password);
                close();
            }
        }
    });
}
