#include <QApplication>
#include <QSharedMemory>

#include "stockwidget.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle("cleanlooks");
    QApplication a(argc, argv);
    QSharedMemory sm{argv[0]};
    if(sm.attach(QSharedMemory::ReadOnly))
    {
        sm.detach();
    }

    if(!sm.create(1))
    {
        qDebug() << "create shared memory failed: " << argv[0];
        return -1;
    }

    Config config;
    if(!config.load(QDir::homePath() + "/stock.json")) {
        return -1;
    }
    StockWidget w{config};
    w.show();
    return a.exec();
}
