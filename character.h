#ifndef CHARACTER_H
#define CHARACTER_H

#include <QJsonObject>
#include <QPixmap>
#include <QDir>
#include <QtMath>

class Character
{
    Character* source;
    QString name;
    QString imagepath;
    QPixmap* image;
    bool ally;
    int hp;
    int curhp;
    int atk;
    int def;
    int mov;
    int range;
    int exp;
    int level;
    int point;

public:
    Character(const QString& name, const QJsonObject& charjson, const bool& ally, const QDir& dir);
    Character(const QJsonObject& charjson, Character* _chara, const QDir& dir);
    Character(const QString& name, const QJsonObject& charjson);
    Character(Character* chara);
    ~Character();
    const QPixmap* getImage() const;
    const QString getName() const;
    const QString getPath() const;
    const bool isAlly() const;
    const int gethp() const;
    const int getcurhp() const;
    const int getatk() const;
    const int getdef() const;
    const int getmov() const;
    const int getrange() const;
    bool isMoved = false;
    bool damage(int atk);
    bool hurt(int damage);

};

#endif // CHARACTER_H
