#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene;
    view = new QGraphicsView(scene, ui->centralwidget);
    view->resize(QSize(1280, 720));
    view->move(QPoint(0, 0));
    view->setTransformationAnchor(QGraphicsView::NoAnchor);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->show();
    QPixmap bg(256, 720);
    bg.fill(Qt::darkCyan);
    ui->label->setPixmap(bg);
    ui->turn->hide();
    ui->vlayout->setMargin(10);
    ui->vlayout->setSpacing(10);
    controller = new GameController(scene, view);
    connect(controller, SIGNAL(clearSidebar()), this, SLOT(onclearSidebar()));
    connect(controller, SIGNAL(displaySidebar(QGraphicsTileItem*)), this, SLOT(ondisplaySidebar(QGraphicsTileItem*)));
    connect(controller, SIGNAL(enemyTurn()), this, SLOT(onEnemyTurn()));
    connect(controller, SIGNAL(allyTurn()), this, SLOT(onAllyTurn()));
    controller->drawNextBattle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onclearSidebar()
{
    QLayoutItem* i;
    while((i = ui->vlayout->takeAt(0)) != nullptr)
    {
        QLayout* layout = dynamic_cast<QLayout*>(i);
        if(layout == nullptr)
        {
            delete i->widget();
        }
        else
        {
            QLayoutItem* j;
            while((j = layout->takeAt(0)) != nullptr)
            {
                delete j->widget();
                delete j;
            }
        }
        delete i;
    }
    view->raise();
    //view->resize(QSize(1280, 720));
    //view->move(QPoint(0, 0));
}

void MainWindow::ondisplaySidebar(QGraphicsTileItem* item)
{
    const Tile* tile = item->getTile();
    const Character* chara = item->getChar();
    onclearSidebar();
    //view->resize(QSize(1024, 720));
    //view->move(QPoint(256, 0));
    QLabel* turntext = new QLabel(this);
    turntext->setText(QString("回合：%1").arg(controller->getTurn()));
    ui->vlayout->addWidget(turntext, 0, Qt::AlignHCenter);
    QHBoxLayout* i = new QHBoxLayout;
    i->addStretch();
    QLabel* bimage = new QLabel(this);
    bimage->setPixmap(*tile->getbImage());
    i->addWidget(bimage);
    QLabel* fimage = new QLabel(this);
    fimage->setPixmap(*tile->getfImage());
    i->addWidget(fimage);
    if(item->getChar() != nullptr)
    {
        QLabel* cimage = new QLabel(this);
        cimage->setPixmap(*chara->getImage());
        i->addWidget(cimage);
    }
    i->addStretch();
    ui->vlayout->addLayout(i);
    QLabel* pass = new QLabel(this);
    pass->setText(tile->getName() + (tile->canPass() ? " 可通行" : " 不可通行"));
    ui->vlayout->addWidget(pass, 0, Qt::AlignHCenter);
    if(tile->canPass())
    {
        QLabel* candd = new QLabel(this);
        candd->setText(QString("消耗：%1").arg(tile->getCost()) + (tile->getDamage() != 0 ? QString(tile->getDamage()>0 ? " 伤害：%1" : " 恢复：%1").arg(tile->getDamage()) : ""));
        ui->vlayout->addWidget(candd, 0, Qt::AlignHCenter);
    }
    if(chara != nullptr)
    {
        QLabel* charname = new QLabel(this);
        charname->setText(chara->getName() + (chara->isAlly() ? QString(" 友方") + (chara->isMoved ? " 已行动" : " 未行动") : " 敌方"));
        ui->vlayout->addWidget(charname, 0, Qt::AlignHCenter);
        QLabel* hp = new QLabel(this);
        hp->setText(QString("生命：%1/%2").arg(chara->getcurhp()).arg(chara->gethp()));
        ui->vlayout->addWidget(hp, 0, Qt::AlignHCenter);
        QLabel* atkdef = new QLabel(this);
        atkdef->setText(QString("攻击：%1 防御：%2").arg(chara->getatk()).arg(chara->getdef()));
        ui->vlayout->addWidget(atkdef, 0, Qt::AlignHCenter);
        QLabel* movrge = new QLabel(this);
        movrge->setText(QString("机动：%1 距离：%2").arg(chara->getmov()).arg(chara->getrange()));
        ui->vlayout->addWidget(movrge, 0, Qt::AlignHCenter);
    }
    ui->vlayout->addStretch();
    ui->label->lower();
    view->lower();
}

void MainWindow::onEnemyTurn()
{
    onclearSidebar();
    ui->turn->setText("敌方行动");
    ui->turn->raise();
    ui->turn->show();
    QTime dieTime= QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents();
    }
    ui->turn->hide();
}

void MainWindow::onAllyTurn()
{
    ui->turn->setText("友方行动");
    ui->turn->raise();
    ui->turn->show();
    QTime dieTime= QTime::currentTime().addSecs(1);
    while(QTime::currentTime() < dieTime)
    {
        QCoreApplication::processEvents();
    }
    ui->turn->hide();
}
