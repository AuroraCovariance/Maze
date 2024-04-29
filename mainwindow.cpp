#include "mainwindow.h"
#include "./ui_mainwindow.h"
#define WALL -1
#define NOTHING 2
#define down 1
#define right 2
#define left 4
#define up 8

extern QString col;
extern QString row;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("迷宫");
    player.load(":/player.jpg");
    colint = col.toInt();
    rowint = row.toInt();
}

void MainWindow::FindBlock()
{
    //找出与当前位置相邻的墙
    if(x_num+1<colint && maze[x_num+1][y_num] == WALL) {//down
        myblock.push_back(block(x_num+1,y_num,down));
    }
    if(y_num+1<rowint && maze[x_num][y_num+1] == WALL) {//right
        myblock.push_back(block(x_num,y_num+1,right));
    }
    if(x_num-1>=0 && maze[x_num-1][y_num] == WALL) {//up
        myblock.push_back(block(x_num-1,y_num,up));
    }
    if(y_num-1>=0 && maze[x_num][y_num-1] == WALL) {//left
        myblock.push_back(block(x_num,y_num-1,left));
    }
}

void MainWindow::generate()
{
    memset(maze,WALL,sizeof(maze));
    //定义起始点
    maze[0][0] = NOTHING;
    start.x = start.y = 0;
    //初始化迷宫数组

    srand((unsigned)time(NULL));

    FindBlock();
    //第一步压入两堵墙（起点右边和起点下面）进入循环
    while(myblock.size()) {
        int BlockSize = myblock.size();
        //随机选择一堵墙（生成0 ~ BlockSize-1之间的随机数，同时也是vector里墙的下标）
        int randnum = rand() % BlockSize;
        block SelectBlock = myblock[randnum];
        x_num = SelectBlock.row;//矿工来到我们“选择的墙”这里
        y_num = SelectBlock.column;
        //根据当前选择的墙的方向进行后续操作
        //此时，起始点 选择的墙 目标块 三块区域在同一直线上
        //我们让矿工从“选择的墙”继续前进到“目标块”
        //矿工有穿墙能力 ：)
        switch(SelectBlock.direction) {
        case down: {
            x_num++;
            break;
        }
        case right: {
            y_num++;
            break;
        }
        case left: {
            y_num--;
            break;
        }
        case up: {
            x_num--;
            break;
        }
        }
        //目标块如果是墙
        if(maze[x_num][y_num]==WALL) {
            //打通墙和目标块
            maze[SelectBlock.row][SelectBlock.column] = maze[x_num][y_num] = NOTHING;
            //再次找出与矿工当前位置相邻的墙
            FindBlock();
        }
        else{//如果不是呢？说明我们的矿工挖到了一个空旷的通路上面 休息一下就好了
            //relax
        }
        //删除这堵墙(把用不了的墙删了，对于那些已经施工过了不必再施工了，同时也是确保我们能跳出循环)
        myblock.erase(myblock.begin()+randnum);
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    QPainter paint(this);//paint.setBrush(QColor(Qt::red));
    int max;
    if(rowint>=colint)
        max=rowint;
    else
        max=colint;

    maze_cell_size=600/max;
    int colLength=this->width()/max;
    int rowLength=this->height()/max;
    for(int i=0;i<colint;++i)
    {
        for(int j=0;j<rowint;++j)
        {
            if(i == start.x && j == start.y)
                painter.drawPixmap(j*maze_cell_size,i*maze_cell_size,maze_cell_size,maze_cell_size,player);
            else if(maze[i][j] == NOTHING)
            {
                paint.setBrush(QColor(Qt::black));
            }
            else
                paint.setBrush(QColor(Qt::white));
            paint.drawRect(j*maze_cell_size,i*maze_cell_size,maze_cell_size,maze_cell_size);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


