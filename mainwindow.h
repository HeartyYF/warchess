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

    GameController* controller;
    QGraphicsScene* scene;
    QGraphicsView* view;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

public slots:
    void onclearSidebar();
    void ondisplaySidebar(QGraphicsTileItem* item);
};
#endif // MAINWINDOW_H
