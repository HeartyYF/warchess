#include "gamecontroller.h"
#include "qcoreapplication.h"

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

struct
{
    bool operator() (QGraphicsTileItem* f, QGraphicsTileItem* s)
    {
        return f->cost + f->eval < s->cost + s->eval;
    }
} cmp;

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
    view->viewport()->update();
    emit enemyTurn();
    turnlist = allylist;
    for(auto& i: allylist)
    {
        i->isMoved = false;
    }
    for(auto& i: enemylist)
    {
        openlist.clear();
        closelist.clear();
        QGraphicsTileItem* begin = QGraphicsTileItem::tilefind[i];
        QGraphicsTileItem* end;
        Character* dest;
        int dist = 999999;
        for(auto& j: allylist)
        {
            QGraphicsTileItem* tempend = QGraphicsTileItem::tilefind[j];
            if(QPoint(begin->x - tempend->x, begin->y - tempend->y).manhattanLength() < dist)
            {
                dist = QPoint(begin->x - tempend->x, begin->y - tempend->y).manhattanLength();
                dest = j;
                end = tempend;
            }
        }
        if(dist <= i->getrange())
        {
            dest->damage(i->getatk());
            QTime dieTime= QTime::currentTime().addSecs(1);
            while(QTime::currentTime() < dieTime)
            {
                QCoreApplication::processEvents();
            }
            continue;
        }
        begin->cost = 0;
        begin->eval = dist;
        end->father = nullptr;
        openlist.emplace_back(begin);
        while(!openlist.empty())
        {
            auto cur = *openlist.begin();
            if(cur->x != 0)
            {
                auto next = QGraphicsTileItem::items[cur->x - 1][cur->y];
                if(next == end)
                {
                    if(end->father == nullptr || end->cost > cur->cost)
                    {
                        end->father = cur;
                        end->cost = cur->cost;
                    }
                }
                else if(next->canPass() && find(closelist.begin(), closelist.end(), next) == closelist.end())
                {
                    if(find(openlist.begin(), openlist.end(), next) != openlist.end())
                    {
                        if(next->cost > cur->cost + next->getTile()->getCost())
                        {
                            next->cost = cur->cost + next->getTile()->getCost();
                            next->father = cur;
                        }
                    }
                    else
                    {
                        next->cost = cur->cost + next->getTile()->getCost();
                        next->eval = QPoint(next->x - end->x, next->y - end->y).manhattanLength();
                        next->father = cur;
                        openlist.emplace_back(next);
                    }
                }
            }
            if(cur->y != 0)
            {
                auto next = QGraphicsTileItem::items[cur->x][cur->y - 1];
                if(next == end)
                {
                    if(end->father == nullptr || end->cost > cur->cost)
                    {
                        end->father = cur;
                        end->cost = cur->cost;
                    }
                }
                else if(next->canPass() && find(closelist.begin(), closelist.end(), next) == closelist.end())
                {
                    if(find(openlist.begin(), openlist.end(), next) != openlist.end())
                    {
                        if(next->cost > cur->cost + next->getTile()->getCost())
                        {
                            next->cost = cur->cost + next->getTile()->getCost();
                            next->father = cur;
                        }
                    }
                    else
                    {
                        next->cost = cur->cost + next->getTile()->getCost();
                        next->eval = QPoint(next->x - end->x, next->y - end->y).manhattanLength();
                        next->father = cur;
                        openlist.emplace_back(next);
                    }
                }
            }
            if(cur->x != QGraphicsTileItem::items.size() - 1)
            {
                auto next = QGraphicsTileItem::items[cur->x + 1][cur->y];
                if(next == end)
                {
                    if(end->father == nullptr || end->cost > cur->cost)
                    {
                        end->father = cur;
                        end->cost = cur->cost;
                    }
                }
                else if(next->canPass() && find(closelist.begin(), closelist.end(), next) == closelist.end())
                {
                    if(find(openlist.begin(), openlist.end(), next) != openlist.end())
                    {
                        if(next->cost > cur->cost + next->getTile()->getCost())
                        {
                            next->cost = cur->cost + next->getTile()->getCost();
                            next->father = cur;
                        }
                    }
                    else
                    {
                        next->cost = cur->cost + next->getTile()->getCost();
                        next->eval = QPoint(next->x - end->x, next->y - end->y).manhattanLength();
                        next->father = cur;
                        openlist.emplace_back(next);
                    }
                }
            }
            if(cur->y != QGraphicsTileItem::items[0].size() - 1)
            {
                auto next = QGraphicsTileItem::items[cur->x][cur->y + 1];
                if(next == end)
                {
                    if(end->father == nullptr || end->cost > cur->cost)
                    {
                        end->father = cur;
                        end->cost = cur->cost;
                    }
                }
                else if(next->canPass() && find(closelist.begin(), closelist.end(), next) == closelist.end())
                {
                    if(find(openlist.begin(), openlist.end(), next) != openlist.end())
                    {
                        if(next->cost > cur->cost + next->getTile()->getCost())
                        {
                            next->cost = cur->cost + next->getTile()->getCost();
                            next->father = cur;
                        }
                    }
                    else
                    {
                        next->cost = cur->cost + next->getTile()->getCost();
                        next->eval = QPoint(next->x - end->x, next->y - end->y).manhattanLength();
                        next->father = cur;
                        openlist.emplace_back(next);
                    }
                }
            }
            openlist.remove(cur);
            closelist.emplace_back(cur);
            openlist.sort(cmp);
        }
        if(end->father == nullptr)
        {
            qDebug()<<"有毛病";
            continue;
        }
        while(end->cost - end->getTile()->getCost() > i->getmov() || end->getChar() != nullptr)
        {
            end = end->father;
        }
        begin->setChara(nullptr);
        end->setChara(i);
        QGraphicsTileItem::tilefind.insert(i, end);
        if(QPoint(QGraphicsTileItem::tilefind[dest]->x - end->x, QGraphicsTileItem::tilefind[dest]->y - end->y).manhattanLength() <= i->getrange())
        {
            dest->damage(i->getatk());
        }
        QTime dieTime= QTime::currentTime().addSecs(1);
        while(QTime::currentTime() < dieTime)
        {
            QCoreApplication::processEvents();
        }
        view->viewport()->update();
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
    emit allyTurn();
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
