#ifndef STOCKWIDGET_H
#define STOCKWIDGET_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QWidget>
#include <QLabel>

#include "config.h"

class StockWidget : public QWidget
{
public:
    StockWidget(const Config & config);

private:
    void timerEvent(QTimerEvent * event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    struct StockItem {
        QString code_;
        QLabel *label_;
        bool ok_;
        QNetworkAccessManager * net_;
        size_t count_ = 0;
    };

    void update(StockItem * item);

    void onUpdate(StockItem * item, QNetworkReply *reply);

    std::vector<StockItem *> items_;
    void initSystemtray();

    QPoint d_;

    bool locked_ = true;
};

#endif // STOCKWIDGET_H
