#include "character.h"
#include "qgraphicstileitem.h"

Character::Character(const QString& charname, const QJsonObject& charjson, const bool& _ally, const QDir& dir)
{
    name = charname;
    imagepath = charjson["image"].toString();
    image = new QPixmap(dir.filePath(imagepath));
    ally = _ally;
    if(!ally)
    {
        hp = charjson["hp"].toInt();
        atk = charjson["atk"].toInt();
        def = charjson["def"].toInt();
        mov = charjson["mov"].toInt();
        range = charjson["range"].toInt();
        curhp = hp;
    }
}

Character::Character(Character* _chara)
{
    source = _chara->source;
    name = _chara->name;
    hp = _chara->hp;
    ally = _chara->ally;
    imagepath = _chara->imagepath;
    image = new QPixmap(*_chara->image);
    atk = _chara->atk;
    def = _chara->def;
    mov = _chara->mov;
    range = _chara->range;
    curhp = hp;
}

Character::Character(const QString& charname, const QJsonObject& charjson)
{
    name = charname;
    ally = true;
    hp = charjson["hp"].toInt();
    atk = charjson["atk"].toInt();
    def = charjson["def"].toInt();
    mov = charjson["mov"].toInt();
    range = charjson["range"].toInt();
}

Character::Character(const QJsonObject& charjson, Character* _chara, const QDir& dir)
{
    source = _chara;
    name = _chara->name;
    imagepath = charjson["image"].toString();
    image = new QPixmap(dir.filePath(imagepath));
    ally = true;
    hp = _chara->hp;
    atk = _chara->atk;
    def = _chara->def;
    mov = _chara->mov;
    range = _chara->range;
    curhp = hp;
}

Character::~Character()
{
    delete image;
}

const QPixmap* Character::getImage() const
{
    return image;
}

const QString Character::getName() const
{
    return name;
}

const QString Character::getPath() const
{
    return imagepath;
}

const bool Character::isAlly() const
{
    return ally;
}

const int Character::gethp() const
{
    return hp;
}

const int Character::getcurhp() const
{
    return curhp;
}

const int Character::getatk() const
{
    return atk;
}

const int Character::getdef() const
{
    return def;
}

const int Character::getmov() const
{
    return mov;
}

const int Character::getrange() const
{
    return range;
}

bool Character::hurt(int damage)
{
    curhp -= damage;
    if(curhp <= 0)
    {
        return true;
    }
    return false;
}

bool Character::damage(int _atk)
{
    int damage;
    if(_atk - def < qCeil(double(_atk) * 0.1))
    {
        damage = qCeil(double(_atk) * 0.1);
        QGraphicsTileItem::defSound.play();
    }
    else
    {
        damage = _atk - def;
        QGraphicsTileItem::atkSound.play();
    }
    return hurt(damage);
}

