#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int interval=60;
    if (argc>1){
        interval=QString(argv[1]).toInt();
    }
    MainWindow w(interval);
    w.setWindowIcon(QIcon(":/charm.png"));
    w.show();
    return a.exec();
}
