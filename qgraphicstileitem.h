#ifndef QGRAPHICSTILEITEM_H
#define QGRAPHICSTILEITEM_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QGraphicsItemGroup>
#include <QGraphicsSceneMouseEvent>
#include <QSoundEffect>
#include <list>
#include <vector>
#include "tile.h"
#include "character.h"
using std::list;
using std::vector;

class QGraphicsTileItem: public QObject, public QGraphicsItemGroup
{
    Q_OBJECT

    QGraphicsPixmapItem bg;
    QGraphicsPixmapItem avatar;
    QGraphicsPixmapItem fg;
    QGraphicsPixmapItem layer;
    Tile* tile;
    Character* chara;
    bool Selected;
    bool canMove;
    bool canAttack;
    int mov;
    int rge;
    static list<QGraphicsTileItem*> layered;
    static QGraphicsTileItem* source;
    static QPointF clickpos;
    static QPointF dragpos;
    static bool isMoved;
    static void clearLayer(bool keepSource = true);
    static void generateLayer(int _x, int _y, int range, bool isMove = true);
public:
    int x, y;
    static QGraphicsView* view;
    static vector<vector<QGraphicsTileItem*>> items;
    static QHash<Character*, QGraphicsTileItem*> tilefind;
    static QSoundEffect atkSound;
    static QSoundEffect defSound;
    QGraphicsTileItem(int i, int j, Tile* _tile);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void setShapeMode(QGraphicsPixmapItem::ShapeMode mode);
    void setChara(Character* _chara);
    bool canPass();
    const static int infdist = -999999;
    const Tile* const getTile() const;
    const Character* const getChar() const;
    int cost;
    int eval;
    QGraphicsTileItem* father;
signals:
    void endChar(Character*);
    void onDeath(Character*);
    void clearSidebar();
    void displaySidebar(QGraphicsTileItem*);
};

#endif // QGRAPHICSTILEITEM_H
