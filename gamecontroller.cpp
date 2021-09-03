#include "gamecontroller.h"

GameController::GameController(QGraphicsScene* _scene, QGraphicsView* _view): scene(_scene), view(_view)
{
    QGraphicsTileItem::view = view;
    QGraphicsTileItem::atkSound.setSource(QUrl::fromLocalFile(":/sound/atk"));
    QGraphicsTileItem::defSound.setSource(QUrl::fromLocalFile(":/sound/def"));
    player = new QMediaPlayer;
    const QString filename = "game/game.json";
    QFile file(filename);
    path = QFileInfo(file).dir();
    file.open(QIODevice::ReadOnly);
    QString value = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject json = document.object();
    const QJsonObject ally = json["ally"].toObject();
    const QStringList allynames = ally.keys();
    for(const QString& name : allynames)
    {
        const auto allyjson = ally[name].toObject();
        Character* chara = new Character(name, allyjson);
        charlist.emplace_back(chara);
        charfind.insert(name, chara);
    }
    battle = json["battle"].toArray();
    map = nullptr;
    current = 0;
}

bool GameController::drawNextBattle()
{
    scene->clear();

    delete map;
    if(current == battle.count())
    {
        return false;
    }
    turn = 1;
    QJsonObject battlejson = battle[current].toObject();
    QString mapfile = battlejson["map"].toString();
    QFile file(path.filePath(mapfile));
    file.open(QIODevice::ReadOnly);
    QString value = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(value.toUtf8());
    QJsonObject mapjson = document.object();
    map = new Map(mapjson, QFileInfo(file).path(), this, player);
    map->summonItems(scene);
    turnlist = allylist;
    return true;
}

Character* GameController::findchar(const QString& name) const
{
    return charfind[name];
}

void GameController::addAlly(Character*& chara)
{
    allylist.emplace_back(chara);
}

void GameController::addEnemy(Character*& chara)
{
    enemylist.emplace_back(chara);
}

void GameController::addWinning(Character*& chara)
{
    winning.emplace_back(chara);
}

void GameController::addLosing(Character*& chara)
{
    losing.emplace_back(chara);
}

void GameController::nextTurn()
{
    turnlist = allylist;
    for(auto& i: allylist)
    {
        i->isMoved = false;
    }
    for(auto& i: allylist)
    {
        if(i->hurt(QGraphicsTileItem::tilefind[i]->getTile()->getDamage()))
        {
            onDeathCheck(i);
        }
    }
    for(auto& i: enemylist)
    {
        if(i->hurt(QGraphicsTileItem::tilefind[i]->getTile()->getDamage()))
        {
            onDeathCheck(i);
        }
    }
    ++turn;
}

void GameController::endTurn(Character* chara)
{
    turnlist.remove(chara);
    if(turnlist.size() == 0)
    {
        nextTurn();
    }
}

void GameController::onDeathCheck(Character* chara)
{
    if(chara->isAlly())
    {
        losing.remove(chara);
        allylist.remove(chara);
        if(losing.size() == 0)
        {
            qDebug()<<"lost";
        }
    }
    else
    {
        winning.remove(chara);
        enemylist.remove(chara);
        if(winning.size() == 0)
        {
            qDebug()<<"win";
        }
    }
}

void GameController::onclearSidebar()
{
    emit clearSidebar();
}

void GameController::ondisplaySidebar(QGraphicsTileItem* item)
{
    emit displaySidebar(item);
}

int GameController::getTurn() const
{
    return turn;
}
