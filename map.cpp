#include "map.h"
#include "gamecontroller.h"

Map::Map(const QJsonObject& mapjson, const QDir curdir, GameController* _controller, QMediaPlayer* player): controller(_controller)
{
    name = mapjson["name"].toString();
    width = mapjson["width"].toInt();
    height = mapjson["height"].toInt();
    win = mapjson["win"].toArray();
    lose = mapjson["lose"].toArray();
    mapinfo.resize(width);
    charinfo.resize(width);
    for(auto& i : mapinfo)
    {
        i.resize(height);
        fill(i.begin(), i.end(), nullptr);
    }
    for(auto& i : charinfo)
    {
        i.resize(height);
        fill(i.begin(), i.end(), nullptr);
    }
    const QString defname = mapjson["default"].toString();
    Tile* defTile;
    const QJsonObject tiles = mapjson["tile"].toObject();
    const QStringList names = tiles.keys();
    for(const QString& name : names)
    {
        const auto tilejson = tiles[name].toObject();
        Tile* tile = new Tile(name, tilejson, curdir);
        if(name == defname)
        {
            defTile = tile;
        }
        tilelist.emplace_back(tile);
        if(tilejson.find("pos") != tilejson.end())
        {
            auto pos = (tiles[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                mapinfo[o["x"].toInt()][o["y"].toInt()] = tile;
            }
        }
    }
    for(auto& i : mapinfo)
    {
        for(auto& j : i)
        {
            if(j == nullptr)
            {
                j = defTile;
            }
        }
    }
    const QJsonObject enemy = mapjson["enemy"].toObject();
    const QStringList enemynames = enemy.keys();
    for(const QString& name : enemynames)
    {
        const auto enemyjson = enemy[name].toObject();
        Character* chara = new Character(name, enemyjson, false, curdir);
        if(enemyjson.find("pos") != enemyjson.end())
        {
            auto pos = (enemy[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                Character* curchar = new Character(chara);
                charinfo[o["x"].toInt()][o["y"].toInt()] = curchar;
                charlist.emplace_back(curchar);
                controller->addEnemy(curchar);
                if(win.contains(QJsonValue(name)))
                {
                    controller->addWinning(curchar);
                }
            }
        }
        delete chara;
    }
    const QJsonObject ally = mapjson["ally"].toObject();
    const QStringList allynames = ally.keys();
    for(const QString& name : allynames)
    {
        const auto allyjson = ally[name].toObject();
        Character* chara = new Character(allyjson, controller->findchar(name), curdir);
        if(allyjson.find("pos") != allyjson.end())
        {
            auto pos = (ally[name].toObject())["pos"].toArray();
            for(const QJsonValue& i : pos)
            {
                auto o = i.toObject();
                Character* curchar = new Character(chara);
                charinfo[o["x"].toInt()][o["y"].toInt()] = curchar;
                charlist.emplace_back(curchar);
                controller->addAlly(curchar);
                if(lose.contains(QJsonValue(name)))
                {
                    controller->addLosing(curchar);
                }
            }
        }
        delete chara;
    }
    if(mapjson.contains("bgm"))
    {
        player->setMedia(QUrl::fromLocalFile(curdir.absoluteFilePath(mapjson["bgm"].toString())));
        player->setVolume(15);
        player->play();
    }
}

Map::~Map()
{
    for(Tile*& tile : tilelist)
    {
        delete tile;
    }
    for(Character*& chara : charlist)
    {
        delete chara;
    }
}


void Map::summonItems(QGraphicsScene*& scene)
{
    QGraphicsTileItem::items.resize(width);
    for(auto i = 0; i < width; ++i)
    {
        QGraphicsTileItem::items[i].resize(height);
        for(auto j = 0; j < height; ++j)
        {
            QGraphicsTileItem* curItem = new QGraphicsTileItem(i, j, mapinfo[i][j]);
            curItem->setShapeMode(QGraphicsPixmapItem::BoundingRectShape);
            curItem->setBoundingRegionGranularity(0.25);
            curItem->setPos(j * 64, i * 64);
            if(charinfo[i][j] != nullptr)
            {
                curItem->setChara(charinfo[i][j]);
                QGraphicsTileItem::tilefind.insert(charinfo[i][j], curItem);
            }
            connect(curItem, SIGNAL(endChar(Character*)), controller, SLOT(endTurn(Character*)));
            connect(curItem, SIGNAL(onDeath(Character*)), controller, SLOT(onDeathCheck(Character*)));
            connect(curItem, SIGNAL(clearSidebar()), controller, SLOT(onclearSidebar()));
            connect(curItem, SIGNAL(displaySidebar(QGraphicsTileItem*)), controller, SLOT(ondisplaySidebar(QGraphicsTileItem*)));
            scene->addItem(curItem);
        }
    }
}

vector<vector<Tile*>> Map::mapinfo;
vector<vector<Character*>> Map::charinfo;
