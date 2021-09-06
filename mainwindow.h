#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gamecontroller.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    GameController* controller;
    QGraphicsScene* scene;
    QGraphicsView* view;
    QMediaPlayer* player;

public slots:
    void onclearSidebar();
    void ondisplaySidebar(QGraphicsTileItem* item);
    void onEnemyTurn();
    void onAllyTurn();
};
#endif // MAINWINDOW_H
