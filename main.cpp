#include "pacmanermaingui.h"

#include <QApplication>
#include <QMetaType>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    PacmanerMainGui w;
    w.show();
    return a.exec();
}
