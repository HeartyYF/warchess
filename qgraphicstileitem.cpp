#include "qgraphicstileitem.h"

QGraphicsTileItem::QGraphicsTileItem(int i, int j, Tile* _tile): tile(_tile), x(i), y(j)
{
    bg.setPixmap(*tile->getbImage());
    fg.setPixmap(*tile->getfImage());
    addToGroup(&bg);
    addToGroup(&avatar);
    addToGroup(&fg);
    addToGroup(&layer);
    Selected = false;
    canMove = false;
    canAttack = false;
    QGraphicsTileItem::items[x][y] = this;
    chara = nullptr;
    mov = infdist;
    rge = infdist;
}

void QGraphicsTileItem::clearLayer(bool keepSource)
{
    if(!keepSource)
    {
        source = nullptr;
    }
    for(auto& i : layered)
    {
        i->layer.setPixmap(QPixmap());
        i->mov = infdist;
        i->rge = infdist;
        i->canMove = false;
        i->canAttack = false;
    }
    layered.clear();
}

void QGraphicsTileItem::generateLayer(int _x, int _y, int range, bool isMove)//处理目前位于 _x _y 的 Tile
{
    range -= isMove ? items[_x][_y]->tile->getCost() : 1;//首先扣除移动到当前格的cost，即使mov不足也能至少移动一格
    //如果是在搜寻敌人 则只扣掉一个
    if(isMove && items[_x][_y]->mov >= range)//如果在行动 需要判断走到这格是否有更优方法
    {
        return;
    }
    if(!isMove && (items[_x][_y]->mov != infdist || items[_x][_y]->rge >= range))//如果在搜敌 则首先判断是否是可以走到的格 然后判断是否有更优搜索
    {
        return;
    }
    if(isMove)
    {
        items[_x][_y]->mov = range;
    }
    else
    {
        items[_x][_y]->rge = range;
    }
    if(isMove)//如果在检测是否可通行
    {
        layered.push_back(items[_x][_y]);
        if(items[_x][_y]->chara == nullptr)
        {
            items[_x][_y]->canMove = true;
            items[_x][_y]->layer.setPixmap(Tile::greenLayer());
        }
        else if(!items[_x][_y]->chara->isAlly())
        {
            layered.push_back(items[_x][_y]);
            if(QPoint(_x - source->x,_y - source->y).manhattanLength() <= source->chara->getrange())
            {
                items[_x][_y]->layer.setPixmap(Tile::redLayer());
                items[_x][_y]->canAttack = true;
            }
            else
            {
                items[_x][_y]->layer.setPixmap(Tile::yellowLayer());
            }
            return;
        }
    }
    else if(items[_x][_y]->chara != nullptr && !items[_x][_y]->chara->isAlly())//如果在检测是否可攻击并且这个位置是敌人
    {
        layered.push_back(items[_x][_y]);
        if(QPoint(_x - source->x,_y - source->y).manhattanLength() <= source->chara->getrange())
        {
            items[_x][_y]->layer.setPixmap(Tile::redLayer());
            items[_x][_y]->canAttack = true;
        }
        else
        {
            items[_x][_y]->layer.setPixmap(Tile::yellowLayer());
        }
    }
    if(range <= 0)//如果走不动了 则接下来检测是否可攻击
    {
        if(!isMove)
        {
            return;
        }
        isMove = false;
        range = source->chara->getrange();
    }
    if(_x != 0)
    {
        if(items[_x - 1][_y]->canPass() || items[_x - 1][_y]->chara != nullptr)
        {
            generateLayer(_x - 1, _y, range, isMove);
        }
    }
    if(_x != items.size() - 1)
    {
        if(items[_x + 1][_y]->canPass() || items[_x + 1][_y]->chara != nullptr)
        {
            generateLayer(_x + 1, _y, range, isMove);
        }
    }
    if(_y != 0)
    {
        if(items[_x][_y - 1]->canPass() || items[_x][_y - 1]->chara != nullptr)
        {
            generateLayer(_x, _y - 1, range, isMove);
        }
    }
    if(_y != items[0].size() - 1)
    {
        if(items[_x][_y + 1]->canPass() || items[_x][_y + 1]->chara != nullptr)
        {
            generateLayer(_x, _y + 1, range, isMove);
        }
    }
}

void QGraphicsTileItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    clickpos = event->screenPos();
    dragpos = event->screenPos();
}

void QGraphicsTileItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if((event->screenPos() - clickpos).manhattanLength() <= 10)
    {
        if(event->button() == Qt::RightButton && !isMoved)
        {
            if(source != nullptr)
            {
                source->Selected = false;
                source->layer.setPixmap(QPixmap());
            }
            clearLayer();
            emit clearSidebar();
            return;
        }
        emit displaySidebar(this);
        view->centerOn(this);
        if(Selected)
        {
            source->layer.setPixmap(QPixmap());
            clearLayer();
            Selected = false;
            this->chara->isMoved = true;
            isMoved = false;
            emit endChar(chara);
        }
        else if(canMove)
        {
            setChara(source->chara);
            source->setChara(nullptr);
            source->Selected = false;
            source->layer.setPixmap(QPixmap());
            tilefind.insert(chara, this);
            clearLayer();
            isMoved = true;
            Selected = true;
            setSelectbox(true);
            source = this;
            chara->isMoved = true;
            generateLayer(x, y, 0);
            emit displaySidebar(this);
        }
        else if(canAttack)
        {
            isMoved = false;
            clearLayer();
            int damage = qMax(source->chara->getatk() - chara->getdef(), qCeil((float)chara->getatk() * 0.1));
            if(chara->damage(source->chara->getatk()))
            {
                emit damaged(damage, x, y);
                emit onDeath(chara);
            }
            else
            {
                emit damaged(damage, x, y);
            }
            if(!isOver)
            {
                source->Selected = false;
                source->layer.setPixmap(QPixmap());
                source->chara->isMoved = true;
                emit endChar(source->chara);
                emit displaySidebar(this);
            }
            else
            {
                isOver = false;
            }
        }
        else if(chara != nullptr && chara->isAlly() && !chara->isMoved && !isMoved)
        {
            isMoved = false;
            if(source != nullptr)
            {
                source->Selected = false;
                source->layer.setPixmap(QPixmap());
            }
            clearLayer();
            Selected = true;
            setSelectbox(true);
            layered.push_back(this);
            source = this;
            generateLayer(x, y, chara->getmov() + tile->getCost());
        }
    }
}

void QGraphicsTileItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    auto trans = event->lastScreenPos() - dragpos;
    if(trans.manhattanLength() > 10)
    {
        dragpos = event->lastScreenPos();
        view->translate(trans.rx(), trans.ry());
    }
}

void QGraphicsTileItem::setShapeMode(QGraphicsPixmapItem::ShapeMode mode)
{
    bg.setShapeMode(mode);
    fg.setShapeMode(mode);
    avatar.setShapeMode(mode);
    layer.setShapeMode(mode);
}

void QGraphicsTileItem::setChara(Character* _chara)
{
    chara = _chara;
    if(chara != nullptr)
    {
        avatar.setPixmap(*chara->getImage());
    }
    else
    {
        avatar.setPixmap(QPixmap());
    }
}

void QGraphicsTileItem::setSelectbox(bool state)
{
    if(state)
    {
        layer.setPixmap(QPixmap(":/image/selectbox"));
    }
    else
    {
        layer.setPixmap(QPixmap());
    }
}

bool QGraphicsTileItem::canPass()
{
    return tile->canPass() && chara == nullptr;
}

const Tile* const QGraphicsTileItem::getTile() const
{
    return tile;
}

const Character* const QGraphicsTileItem::getChar() const
{
    return chara;
}

QPointF QGraphicsTileItem::clickpos;
QPointF QGraphicsTileItem::dragpos;
QGraphicsView* QGraphicsTileItem::view = nullptr;
QGraphicsTileItem* QGraphicsTileItem::source = nullptr;
vector<vector<QGraphicsTileItem*>> QGraphicsTileItem::items;
list<QGraphicsTileItem*> QGraphicsTileItem::layered;
QHash<Character*, QGraphicsTileItem*> QGraphicsTileItem::tilefind;
bool QGraphicsTileItem::isMoved = false;
bool QGraphicsTileItem::isOver = false;
QSoundEffect QGraphicsTileItem::atkSound;
QSoundEffect QGraphicsTileItem::defSound;
