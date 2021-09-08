#include "levelup.h"
#include "ui_levelup.h"

Levelup::Levelup(list<Character*> _charlist, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Levelup),
    charlist(_charlist)
{
    ui->setupUi(this);
    for(Character*& chara : charlist)
    {
        chara->pt += 10;
        QListWidgetItem* item = new QListWidgetItem();
        item->setSizeHint(QSize(64, 100));
        ui->list->addItem(item);
        auto nameLabel = new QLabel();
        nameLabel->setText(QString("   ") + chara->getName());
        ui->list->setItemWidget(item, nameLabel);
        charfind.insert(item, chara);
    }
}

Levelup::~Levelup()
{
    delete ui;
}

void Levelup::on_next_clicked()
{
    emit end(this);
}


void Levelup::on_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    curchar = charfind[current];
    ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
    ui->atk->setText(QString("攻击：%1").arg(curchar->atk));
    ui->def->setText(QString("防御：%1").arg(curchar->def));
    ui->mov->setText(QString("机动：%1").arg(curchar->mov));
    ui->hp->setText(QString("生命：%1").arg(curchar->hp));
    ui->rge->setText(QString("范围：%1").arg(curchar->range));

}


void Levelup::on_atkb_clicked()
{
    if(curchar != nullptr && curchar->pt > 0)
    {
        curchar->atk += 1;
        curchar->pt -= 1;
        ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
        ui->atk->setText(QString("攻击：%1").arg(curchar->atk));
    }
}


void Levelup::on_defb_clicked()
{
    if(curchar != nullptr && curchar->pt > 0)
    {
        curchar->def += 1;
        curchar->pt -= 1;
        ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
        ui->def->setText(QString("防御：%1").arg(curchar->def));
    }
}


void Levelup::on_movb_clicked()
{
    if(curchar != nullptr && curchar->pt > 4)
    {
        curchar->mov += 1;
        curchar->pt -= 5;
        ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
        ui->mov->setText(QString("机动：%1").arg(curchar->mov));
    }
}


void Levelup::on_hpb_clicked()
{
    if(curchar != nullptr && curchar->pt > 0)
    {
        curchar->hp += 2;
        curchar->pt -= 1;
        ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
        ui->hp->setText(QString("生命：%1").arg(curchar->hp));
    }
}


void Levelup::on_rgeb_clicked()
{
    if(curchar != nullptr && curchar->pt > 9)
    {
        curchar->range += 1;
        curchar->pt -= 10;
        ui->pt->setText(QString("可用加点：%1").arg(curchar->pt));
        ui->rge->setText(QString("范围：%1").arg(curchar->range));
    }
}
