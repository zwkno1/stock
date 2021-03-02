#include <QApplication>

#include "stockwidget.h"




int main(int argc, char *argv[])
{
    QApplication::setStyle("cleanlooks");
    QApplication a(argc, argv);

    Config config;
    if(!config.load(QDir::homePath() + "/stock.json")) {
        return -1;
    }
    StockWidget w{config};
    w.show();
    return a.exec();
}
