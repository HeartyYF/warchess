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
    void stopGame();
    void onNewTurn(int);
    void showName(QString);
    void onLevelup(list<Character*>);
    void onEndLevelup(Levelup*);

private slots:
    void on_pushButton_clicked();
};
#endif // MAINWINDOW_H
