#include "dialog.h"

Dialog::Dialog(const QJsonArray& dialjson, const QDir curdir, QGraphicsScene* scene, QMediaPlayer* _player): player(_player), dialarray(dialjson), dir(curdir)
{
    addToGroup(&bg);
    addToGroup(&text);
    text.setPos(240, 500);
    scene->addItem(this);
    count = 0;
    dial = 0;
    loadCurDial();
}

void Dialog::loadCurDial()//其实是加载下一段对话
{
    if(count == dialarray.count())
    {
        emit dialogEnd();
        return;
    }
    if(dial == 0)//读取可选的bgm和背景图
    {
        if(dialarray[count].toObject().contains("bgm"))
        {
            player->setMedia(QUrl::fromLocalFile(dir.absoluteFilePath(dialarray[count].toObject()["bgm"].toString())));
            player->setVolume(15);
            player->play();
        }
        if(dialarray[count].toObject().contains("pic"))
        {
            QString imagepath = dialarray[count].toObject()["pic"].toString();
            QPixmap* image = new QPixmap(dir.filePath(imagepath));
            bg.setPixmap(*image);
            delete image;
        }
    }
    text.setPlainText(dialarray[count].toObject()["dial"].toArray()[dial].toString());
    ++dial;
    if(dial == dialarray[count].toObject()["dial"].toArray().count())
    {
        ++count;
        dial = 0;
    }
}

void Dialog::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    loadCurDial();
}
