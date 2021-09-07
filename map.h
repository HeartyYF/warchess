#ifndef MAP_H
#define MAP_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QListWidget>
#include <QDir>
#include <QHBoxLayout>
#include <QLabel>
#include <QGraphicsScene>
#include <QMap>
#include <QObject>
#include <QMediaPlayer>
#include <list>
#include <vector>
#include <algorithm>
#include "qgraphicstileitem.h"
#include "tile.h"
#include "character.h"
using std::list;
using std::vector;
using std::fill;

class GameController;

class Map: public QObject
{
    GameController* controller;
    list<Tile*> tilelist;
    list<Character*> charlist;
    QJsonArray win;
    QJsonArray lose;
    int width;
    int height;

public:
    Map(const QJsonObject& mapjson, const QDir curdir, GameController* _controller, QMediaPlayer* player);
    ~Map();
    void summonItems(QGraphicsScene*& scene);
    static vector<vector<Tile*>> mapinfo;
    static vector<vector<Character*>> charinfo;
    QString name;
};

#endif // MAP_H
