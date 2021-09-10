#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMap>
#include <QMediaPlayer>
#include <list>
#include <algorithm>
#include "map.h"
#include "dialog.h"
#include "levelup.h"
using std::list;
using std::find;

class GameController: public QObject
{
    Q_OBJECT

    QGraphicsScene* scene;
    QGraphicsView* view;
    Map* map;
    Dialog* dialog;
    list<Character*> charlist;//用于存放所有友方的metadata
    list<Character*> allylist;//战斗中实例化的友方
    list<Character*> enemylist;//战斗中实例化的敌方
    list<Character*> winning;//胜利条件
    list<Character*> losing;//失败条件
    list<Character*> turnlist;
    list<QGraphicsTileItem*> openlist;//用于a*寻路
    list<QGraphicsTileItem*> closelist;
    QMap<QString, Character*> charfind;
    QMediaPlayer* player;
    QDir path;
    QJsonArray battle;
    int current;
    int turn;
    void nextTurn();

public:
    GameController(QGraphicsScene* _scene, QGraphicsView* _view, QMediaPlayer* _player);
    Character* findchar(const QString& name) const;
    void addAlly(Character*&);
    void addEnemy(Character*&);
    void addWinning(Character*&);
    void addLosing(Character*&);
    int getTurn() const;
    void drawAfterDialog();

public slots:
    void endTurn(Character*);
    void onDeathCheck(Character*);
    void onclearSidebar();
    void ondisplaySidebar(QGraphicsTileItem*);
    void drawBeforeDialog();
    void drawNextBattle();
    void drawLevelup();
    void damageText(int, int, int);

signals:
    void clearSidebar();
    void displaySidebar(QGraphicsTileItem*);
    void enemyTurn();
    void allyTurn();
    void gameWin();
    void gameLost();
    void newTurn(int);
    void showMap(QString);
    void levelup(list<Character*>);

};

#endif // GAMECONTROLLER_H
