#ifndef CONFIG_H
#define CONFIG_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QDir>
#include <QPoint>
#include <QJsonObject>


struct Config
{
    QStringList stocks_;
    QPoint pos_;

    bool load(QString filename) {
        QFile file(filename);
        qDebug() << "load config: " << filename;
        if(!file.open(QIODevice::ReadOnly))
        {
            qDebug() << "open config file failed: " << filename;
            return false;
        }

        QJsonParseError e;
        auto content = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(content, &e);
        if(e.error != QJsonParseError::NoError || !doc.isObject())
        {
            qDebug() << "config format error: " << content;
            return false;
        }

        auto stocks = doc.object()["stocks"];
        if(!stocks.isArray()) {
            return false;
        }

        for(auto i : stocks.toArray()) {
            if(i.isString()) {
                stocks_.push_back(i.toString());
            }
        }

        auto pos = doc.object()["pos"];
        if(pos.isObject()) {
            auto x = pos.toObject()["x"];
            auto y = pos.toObject()["y"];
            if(x.isDouble() && y.isDouble()) {
                pos_.setX(x.toInt());
                pos_.setY(y.toInt());
            }
        }
        return true;
    }
};

#endif // CONFIG_H
