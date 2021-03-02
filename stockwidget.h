#ifndef STOCKWIDGET_H
#define STOCKWIDGET_H

#include <QNetworkAccessManager>
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include "config.h"

class StockWidget : public QWidget
{
public:
    StockWidget(const Config & config);

    void timerEvent(QTimerEvent * event) override;

private:
    struct StockItem {
        QString code_;
        QLabel *label_;
        bool ok_;
        QNetworkAccessManager * net_;
    };

    void update(StockItem * item);

    void onUpdate(StockItem * item, QNetworkReply *reply);

    std::vector<StockItem *> items_;
};

#endif // STOCKWIDGET_H
