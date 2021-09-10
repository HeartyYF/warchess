#ifndef LEVELUP_H
#define LEVELUP_H

#include <QWidget>
#include <QListWidgetItem>
#include <list>
#include "character.h"
using std::list;

namespace Ui {
class Levelup;
}

class Levelup : public QWidget
{
    Q_OBJECT

public:
    Levelup(list<Character*>, QWidget *parent = nullptr);
    ~Levelup();

signals:
    void end(Levelup*);

private slots:
    void on_next_clicked();
    void on_list_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_atkb_clicked();
    void on_defb_clicked();
    void on_movb_clicked();
    void on_hpb_clicked();
    void on_rgeb_clicked();

private:
    Ui::Levelup *ui;
    list<Character*> charlist;
    QHash<QListWidgetItem*, Character*> charfind;
    Character* curchar = nullptr;
};

#endif // LEVELUP_H
