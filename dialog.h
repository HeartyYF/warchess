#ifndef DIALOG_H
#define DIALOG_H

#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QObject>
#include <QDir>
#include <QMediaPlayer>
#include <QJsonObject>
#include <QJsonArray>

class Dialog: public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    QMediaPlayer* player;
    QJsonArray dialarray;
    QGraphicsPixmapItem bg;
    QGraphicsTextItem text;
    QDir dir;
    int count;
    int dial;
    void loadCurDial();

public:
    Dialog(const QJsonArray& dialjson, const QDir curdir, QGraphicsScene* scene, QMediaPlayer* player);
    void mousePressEvent(QGraphicsSceneMouseEvent*);

signals:
    void dialogEnd();
};

#endif // DIALOG_H
