#ifndef TILE_H
#define TILE_H

#include <QJsonObject>
#include <QPixmap>
#include <QDir>

class Tile
{
    QString name;
    QString fimagepath;
    QPixmap* fimage;
    QString bimagepath;
    QPixmap* bimage;
    bool pass;
    int cost;
    int damage;

public:
    Tile(const QString& name, const QJsonObject& tilejson, const QDir& dir);
    ~Tile();
    const QPixmap* getbImage() const;
    const QPixmap* getfImage() const;
    const QString getName() const;
    const QString getbPath() const;
    const QString getfPath() const;
    const bool canPass() const;
    const int getCost() const;
    const int getDamage() const;
    static QPixmap nullImage();
    static QPixmap redLayer();
    static QPixmap yellowLayer();
    static QPixmap greenLayer();

};

#endif // TILE_H
