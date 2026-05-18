#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>
#include <QMouseEvent>

//나는 이프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다


class MyButton: public QPushButton
{
 Q_OBJECT

private:
    bool flagon=false;
    bool opened=false;
    void restoreFlag();

public:
    static int flags; //remaining flags
    static int opencnt; //opened box
    static int winlose; //win(1) lose(-1) normal(0)
    MyButton(QWidget *parent = nullptr);
    ~MyButton();
    int cx,cy; //current position
    int type=0; //-1 for mine, other for num
    QString label;
    void open();
private slots:
    void rclick();
    void lclick();
protected:
 void mousePressEvent(QMouseEvent *e) override;
signals:
 void rightClick();
 void frefresh(); //flag counter refresh
 void lost(); //game lose
 void firstClick(int,int);
 void opensignal(int,int);
};

#endif // MYBUTTON_H


