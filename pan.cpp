#include "pan.h"

//나는 이프로그래밍 과제를 다른 사람의 부적절한 도움 없이 완수하였습니다

namespace {
std::default_random_engine randomEngine()
{
    const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    return std::default_random_engine(static_cast<unsigned>(seed));
}
}

Pan::Pan(int n, QLabel* _flagcnt,Timego* time, QWidget *parent):
    QWidget(parent)
  ,level(n),flagw(_flagcnt),timer(time){
    glay=new QGridLayout(this);
    switch(level){
    case 0:
        x=10; y=8;
        mines=10;
        setStyleSheet("MyButton { background: rgb(255,179,0); color: black; border: 1px solid rgb(65,65,58); margin: 0px; padding: 0px; font-size: 28px; }");
        break;
    case 1:
        x=18; y=14;
        mines=40;
        setStyleSheet("MyButton { background: rgb(255,179,0); color: black; border: 1px solid rgb(65,65,58); margin: 0px; padding: 0px; font-size: 16px; }");
        break;
    case 2:
        x=24; y=20;
        mines=99;
        setStyleSheet("MyButton { background: rgb(255,179,0); color: black; border: 1px solid rgb(65,65,58); margin: 0px; padding: 0px; font-size: 12px; }");
        break;
    }
    MyButton::flags=mines;
    flagw->setText(QString(" Flags: %1").arg(MyButton::flags));

    glay->setHorizontalSpacing(2);
    glay->setVerticalSpacing(2);
    for(int j=0;j<x;j++)
        glay->setColumnStretch(j,1); //equal column width
    for(int i=0;i<y;i++)
        glay->setRowStretch(i,1); //equal row height

    bts = new MyButton* [y];
    for (int i = 0; i < y; i++)
        bts[i] = new MyButton[x];

    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            bts[i][j].setMinimumSize(30,30);
            QSizePolicy temp = bts[i][j].sizePolicy();
            temp.setVerticalPolicy(temp.horizontalPolicy());
            bts[i][j].setSizePolicy(temp); //design optimization

            glay->addWidget(&(bts[i][j]),i,j);
            bts[i][j].cx=j;
            bts[i][j].cy=i; //self position info
            bts[i][j].setText(QString(" "));
            connect(&bts[i][j],SIGNAL(frefresh()),this,SLOT(frefresh_slot()));
            connect(&bts[i][j],SIGNAL(lost()),this,SLOT(lost_slot()));
            connect(&bts[i][j],SIGNAL(firstClick(int,int)),this,SLOT(first_click_slot(int,int)));
            connect(&bts[i][j],SIGNAL(opensignal(int,int)),this,SLOT(open_slot(int,int)));
        }
    }

    std::vector<std::pair<int,int>> foo; //random mine algorithm begin
    foo.reserve(x * y);
    for(int i=0;i<y;++i){
        for(int j=0;j<x;++j){
            foo.push_back(std::make_pair(i,j));
        }
    } //button position list vector
    auto engine = randomEngine();
    std::shuffle(foo.begin(), foo.end(), engine); //first some element become mine
    for(int p=0;p<mines;++p){
        bts[foo[p].first][foo[p].second].type=-1;
        bts[foo[p].first][foo[p].second].label="!";

    }
    refreshNumbers();
} //end constructor

Pan::~Pan(){
    for (int i = 0; i < y; i++)
        delete[] bts[i];
    delete[] bts;
    delete glay;
}

bool Pan::isInside(int cx, int cy) const
{
    return cx >= 0 && cx < x && cy >= 0 && cy < y;
}

int Pan::countAdjacentMines(int cx, int cy) const
{
    int sum=0;
    for(int dy=-1;dy<=1;dy++){
        for(int dx=-1;dx<=1;dx++){
            if(dx==0 && dy==0)
                continue;

            const int nx=cx+dx;
            const int ny=cy+dy;
            if(isInside(nx,ny) && bts[ny][nx].type==-1)
                ++sum;
        }
    }
    return sum;
}

void Pan::refreshNumbers()
{
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(bts[i][j].type!=-1){
                const int sum=countAdjacentMines(j,i);
                bts[i][j].type=sum; bts[i][j].label=QString::number(sum); //surrounding(8) mine count inject
            }
        }
    }
}

void Pan::frefresh_slot(){ //refresh
    flagw->setText(QString(" Flags: %1").arg(MyButton::flags)); //flag refresh
    timer->gogo();
    if(MyButton::opencnt==x*y-mines){ //whether all opened
        MyButton::winlose=1;
        for(int i=0;i<y;i++){
            for(int j=0;j<x;j++){
                bts[i][j].open();
            }
        }
        flagw->setText(" You Win !");
        timer->stop();
    }
}
void Pan::lost_slot(){
    MyButton::winlose=-1;
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(bts[i][j].type==-1)
            bts[i][j].open(); //open all mine
        }
    }
    flagw->setText(" You Lost !");
    timer->stop();

}

void Pan::first_click_slot(int cx,int cy)
{
    if(MyButton::opencnt != 0 || bts[cy][cx].type != -1)
        return;

    std::vector<std::pair<int,int>> spots;
    spots.reserve(x * y - mines);
    for(int i=0;i<y;i++){
        for(int j=0;j<x;j++){
            if(!(i==cy && j==cx) && bts[i][j].type!=-1)
                spots.push_back(std::make_pair(i,j));
        }
    }

    auto engine = randomEngine();
    std::uniform_int_distribution<std::size_t> distribution(0, spots.size()-1);
    const auto& spot=spots[distribution(engine)];
    int ny=spot.first;
    int nx=spot.second;
    bts[cy][cx].type=0;
    bts[cy][cx].label="0";
    bts[ny][nx].type=-1;
    bts[ny][nx].label="!";
    refreshNumbers();
}

void Pan::open_slot(int cx,int cy){ //check and open surrounding 8 blocks
    for(int dy=-1;dy<=1;dy++){
        for(int dx=-1;dx<=1;dx++){
            if(dx==0 && dy==0)
                continue;

            const int nx=cx+dx;
            const int ny=cy+dy;
            if(isInside(nx,ny))
                bts[ny][nx].open();
        }
    }

}


