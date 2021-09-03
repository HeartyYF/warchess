#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QMediaPlayer>
#include <list>
#include "map.h"
using std::list;

class GameController: public QObject
{
    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsView* view;
    Map* map;
    list<Character*> charlist;//用于存放所有友方的metadata
    list<Character*> allylist;//战斗中实例化的友方
    list<Character*> enemylist;//战斗中实例化的敌方
    list<Character*> winning;//胜利条件
    list<Character*> losing;//失败条件
    list<Character*> turnlist;
    QMap<QString, Character*> charfind;
    QMediaPlayer* player;
    QDir path;
    QJsonArray battle;
    int current;
    int turn;
    void nextTurn();

public:
    GameController(QGraphicsScene* _scene, QGraphicsView* _view);
    bool drawNextBattle();
    Character* findchar(const QString& name) const;
    void addAlly(Character*&);
    void addEnemy(Character*&);
    void addWinning(Character*&);
    void addLosing(Character*&);
    int getTurn() const;

public slots:
    void endTurn(Character*);
    void onDeathCheck(Character*);
    void onclearSidebar();
    void ondisplaySidebar(QGraphicsTileItem*);

signals:
    void clearSidebar();
    void displaySidebar(QGraphicsTileItem*);

};

#endif // GAMECONTROLLER_H
