#include "stockwidget.h"

#include <QNetworkReply>
#include <QVBoxLayout>
#include <QFontDatabase>
#include <QSystemTrayIcon>
#include <QMenu>
#include "ui_stockwidget.h"


struct StockInfo {
    QString close;
    QString open;
    QString curr;
    QString date;
    QString time;
    QString rate;
    QString bar;

    bool parse(const QString & str) {
        auto arr = str.split(",");
        if(arr.size() < 33) {
            return false;
        }
        close = arr[2];
        open = arr[1];
        curr = arr[3];
        date = arr[30];
        time = arr[31];
        rate = QString::number((curr.toDouble() - close.toDouble())*100/close.toDouble(), 'f', 2);
        return true;
    }

    QString fmt(QString str, int len = 8) {
        if(str.size() >= len) {
            return str;
        }
        return QString(len - str.size(), ' ') + str;
    }

    QString str() {
        const static QString sep = " ";
        QString str;
        str += time + bar + sep;
        str += fmt(close) + sep;
        str += fmt(open) + sep;
        str += fmt(curr) + sep;
        str += fmt(rate);
        return str;
    }
};

StockWidget::StockWidget(const Config & config)
{
    initSystemtray();
    auto & stocks = config.stocks_;
    qDebug() << stocks;
    this->setGeometry(config.pos_.x(), config.pos_.y(), 240, 16 * stocks.size());
    //this->setFixedSize();
    this->setWindowOpacity(1);
    this->setWindowFlags(Qt::FramelessWindowHint |  Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint
#ifdef Q_OS_MAC
                         | Qt::WindowSystemMenuHint
#else
                         | Qt::Tool
#endif
    );

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setMargin(0);
    this->setLayout(layout);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    font.setPixelSize(12);
    qDebug() << font;
    for(int i = 0; i < stocks.size(); ++i) {
        auto label = new QLabel(this);
        label->resize(240, 16);
        label->setAlignment(Qt::AlignLeft);
        label->setFont(font);
        label->setMargin(0);
        layout->addWidget(label);
        auto item = new StockItem{};
        item->code_ = stocks[i];
        item->label_ = label;
        item->net_ = new QNetworkAccessManager(this);
        item->ok_ = true;
        items_.push_back(item);
        connect(item->net_, &QNetworkAccessManager::finished, [item, this](QNetworkReply *reply){
            this->onUpdate(item, reply);
        });
    }

    this->startTimer(1000);
}

void StockWidget::initSystemtray()
{
    QSystemTrayIcon *tray = new QSystemTrayIcon(this);

    QIcon icon("/Users/oooh/icon.png");
    tray->setIcon(icon);
    tray->setVisible(true);

    QMenu *menu = new QMenu(this);
    tray->setContextMenu(menu);
    QAction *a1 = new QAction("Quit", this);
    menu->addAction(a1);
    connect(a1, SIGNAL(triggered()), this, SLOT(close()));
    //
    //QAction *a2 = new QAction("tray menu item 2", this);
    //stiMenu->addAction(a2);
    //connect(a2, SIGNAL(triggered()), this, SLOT(action2Fired()));
    tray->show();
}

void StockWidget::timerEvent(QTimerEvent *event)
{
    for(auto & i : items_) {
        update(i);
    }
}

void StockWidget::update(StockItem * item)
{
    if(!item->ok_) {
        return;
    }
    item->ok_ = false;


    QNetworkRequest request;
    request.setUrl(QUrl("http://hq.sinajs.cn/list=" + item->code_));
    item->net_->get(request);

}

void StockWidget::onUpdate(StockItem * item, QNetworkReply * reply)
{
    ++item->count_;
    StockInfo info;
    info.bar = R"(-/|\)"[item->count_%4];
    info.parse(reply->readAll());
    qDebug() << info.str();
    auto label = item->label_;
    label->setText(item->code_ + " " + info.str());
    qDebug() << label->size();
    item->ok_ = true;
}
