#ifndef TIMEGO_H
#define TIMEGO_H

//나는 이프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다


#include <QLabel>
#include <QObject>
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

class Timego : public QLabel{
    Q_OBJECT
private:
    QTimer timer; //time update signal
    QElapsedTimer timex; //timer with 100ms too inaccurate
public:
    Timego(QWidget* para = nullptr):QLabel(para){
        connect(&timer,SIGNAL(timeout()),this,SLOT(up()));
    }
    bool running=false;
public slots:
    void gogo();
    void zero();
    void stop();
    void up();
}; //time label end


#endif // TIMEGO_H

