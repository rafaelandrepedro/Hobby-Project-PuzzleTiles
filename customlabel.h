#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QTimer>

#include "utils.h"
#include "game.h"







class CustomLabel : public QLabel
{
    Q_OBJECT

public:
    Game game;
    bool XButton1;
    Coord offsetStartPoint;
    Coord offsetOriginalPoint;

    QTimer *timer;

    CustomLabel(QWidget* parent = nullptr) : QLabel(parent) {
        setMouseTracking(true);
        //play("battlesong.mp3");
        XButton1=false;


        // Create a QTimer instance
        timer = new QTimer(this);

        // Connect the timeout signal of the timer to your custom slot
        connect(timer, &QTimer::timeout, this, &CustomLabel::onTimeout);

        // Set the interval for the timer (in milliseconds)
        timer->start(20);
    }



protected:

    void onTimeout()
    {
        game.runTick();
        //std::cout<<"timer"<<std::endl;
    }



    void paintEvent(QPaintEvent* event) override {
        parentWidget()->resize(960, 720);
        QLabel::paintEvent(event);

        // Now, you can draw additional content on top of the image.
        QPainter painter(this);

        game.paint(painter);


        update();
    }

    void mousePressEvent(QMouseEvent* event) override {
        game.mouseEvent(event);
        update();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        game.mouseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        game.mouseEvent(event);
        update();
    }

    void wheelEvent(QWheelEvent *event){
        game.wheelEvent(event);
    }

    void resizeEvent(QResizeEvent* event) override {

    }
};



#endif // CUSTOMLABEL_H
