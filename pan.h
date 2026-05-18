#ifndef PAN_H
#define PAN_H

//나는 이프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다


#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QSizePolicy>
#include <algorithm>
#include <random>
#include <chrono>
#include <utility>
#include <vector>
#include "timego.h"
#include "mybutton.h"


class Pan: public QWidget{
    Q_OBJECT
private:
    int level;
    QGridLayout *glay;
    MyButton ** bts;
    int x,y;
    int mines;
    //int flags;
    //int opens;
    QLabel* flagw; //flag widget
    Timego* timer;
    bool isInside(int cx, int cy) const;
    int countAdjacentMines(int cx, int cy) const;
    void refreshNumbers();
public:
    Pan(int n, QLabel* _flagcnt,Timego* time, QWidget *parent=nullptr);
    ~Pan();
private slots:
    void frefresh_slot();
    void lost_slot();
    void first_click_slot(int cx,int cy);
    void open_slot(int cx,int cy);
};

#endif // PAN_H


