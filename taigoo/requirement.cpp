#include "requirement.h"
#include "ui_requirement.h"
#include <QDebug>
Requirement::Requirement(QWidget *parent) :
    QMainWindow(parent),
    good(0),
    normal(0),
    bad(0),
    score(0),
    ui(new Ui::Requirement),
    gif(new QMovie),
    t(new QTimer),
    tempo(10),
    timer(0),
    result(new Result)
{
    ui->setupUi(this);
    gif->setFileName(":/gif/image/gif/miku.gif");
    ui->label->setMovie(gif);
    gif->setSpeed(250);
    gif->start();
    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();
    connect(t, SIGNAL(timeout()), this, SLOT(timing()));
    srand(time(NULL));
    gs = new QGraphicsScene(0,0,1024,291);
    ui->graphicsView->setScene(gs);
}

Requirement::~Requirement()
{
    delete ui;
    delete gif;
    delete t;
    delete gs;
    delete result;
}

void Requirement::keyPressEvent(QKeyEvent *k)
{

    //qDebug() << k->key();
    if(determine == spectral.end())
        return;
    if((*determine)->x() < 400)
    {
        int color = (*determine)->getDrum();
        if(k->key()==Qt::Key_G || k->key()==Qt::Key_H)
        {
            qDebug() << "Red hit detected";
            if(color == 0 || color == 2)
            {
                count_score((*determine)->x());
                delete *determine;
                ++determine;
            }
            else if(color == 1 || color == 3)
            {
                qDebug() << "Wrong color";
                ++bad;
                --score;
                ui->label_6->hide();
                ui->label_7->hide();
                ui->label_8->show();
                ui->lcdNumber_3->display(score);
                delete *determine;
                ++determine;
            }
        }
        if(k->key()==Qt::Key_F || k->key()==Qt::Key_J)
        {
            qDebug() << "Blue hit detected";
            if(color == 1 || color == 3)
            {
                count_score((*determine)->x());
                delete *determine;
                ++determine;
            }
            else if(color == 0 || color == 2)
            {
                qDebug() << "Wrong color";
                ++bad;
                --score;
                ui->label_6->hide();
                ui->label_7->hide();
                ui->label_8->show();
                ui->lcdNumber_3->display(score);
                delete *determine;
                ++determine;
            }
        }
    }
    qDebug() << "Score : " << score;
    qDebug() << "Good : " << good;
    qDebug() << "Normal : " << normal;
    qDebug() << "Bad : " << bad;
    qDebug() << "--------------------";
}

void Requirement::setSpectral()
{
    int i;
    for(i=0; i < 110; ++i)
    {
        d = new drum(rand()%4, i*800);
        spectral.push_back(d);
    }
    it = spectral.begin();
    determine = spectral.begin();
}

void Requirement::count_score(int location)
{
    int acc = 270;
    int mis = abs(location - acc);
    if(mis < 10)
    {
        qDebug() << "Right";
        score += 2;
        ++good;
        ui->label_7->hide();
        ui->label_8->hide();
        ui->label_6->show();
    }
    else if(mis>=10 && mis <50)
    {
        qDebug() << "Not quite right";
        ++score;
        ++normal;
        ui->label_6->hide();
        ui->label_8->hide();
        ui->label_7->show();
    }
    else
    {
        qDebug() << "Missed";
        --score;
        ++bad;
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->show();
    }
    ui->lcdNumber_3->display(score);
}

void Requirement::starter()
{
    spectral.clear();
    setSpectral();
    ui->graphicsView->show();
    t->start(tempo);
}

void Requirement::timing()
{
    timer += tempo;
    /*if(timer <= 10000)
    {
        ui->lcdNumber_2->display(10-(timer/1000));
        if(it != spectral.end())
        {
            if(timer >= (*it)->getStartTime())
            {
                gs->addItem(*it);
                connect(t, SIGNAL(timeout()), *it, SLOT(move()));
                connect((*it), SIGNAL(destroy_drum()), this, SLOT(deleted()));
                ++it;
            }
        }
    }*/
    if(timer <= 30000)
    {
        ui->lcdNumber_2->display(30-(timer/1000));
        if(it != spectral.end())
        {
            if(timer >= (*it)->getStartTime())
            {
                gs->addItem(*it);
                connect(t, SIGNAL(timeout()), *it, SLOT(move()));
                connect((*it), SIGNAL(destroy_drum()), this, SLOT(deleted()));
                ++it;
            }
        }
    }
    else
    {
        show_result();
    }
}

void Requirement::deleted()
{
    if(determine != spectral.end())
        ++determine;
}

void Requirement::show_result()
{
    t->stop();
    this->hide();
    gs->clear();
    result->show();
    result->showScore(score);
    result->showGood(good);
    result->showNormal(normal);
    result->showBad(bad);
    if(result->exec())
    {
        qDebug() << result->exec();
        timer = 0;
        score = 0;
        good = 0;
        normal = 0;
        bad = 0;
        ui->lcdNumber_3->display(0);
        ui->label_6->hide();
        ui->label_7->hide();
        ui->label_8->hide();
        this->show();
        starter();
    }
}


