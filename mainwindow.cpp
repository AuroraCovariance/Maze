#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
    generate();
}


void MainWindow::generate()
{
    int i=3,j=3;
    maze[i][j].state=1;
    point temp;
    temp.i=i;
    temp.j=j;
    temp.state=1;
    bool up=false,down=false,left=false,right=false;

    srand((unsigned)time(NULL));

    while(true){//利用prim算法生成迷宫
        temp.i=i;
        temp.j=j;

        int randnum=rand()%4;

        switch (randnum) {
        case 0: if(!up&&i>2&&maze[i-2][j].state==0){
                build_maze_stack.push(temp);
                maze[i-2][j].state=1;
                maze[i-1][j].state=1;
                i=i-2;
                if(maze[i-1][j].state==0)
                    up=false;
                else
                    up=true;
                if(maze[i+1][j].state==0)
                    down=false;
                else
                    down=true;
                if(maze[i][j-1].state==0)
                    left=false;
                else
                    left=true;
                if(maze[i][j+1].state==0)
                    right=false;
                else
                    right=true;
            }
            else{
                up=true;
            }
            break;
        case 1: if(!down&&i<rowint-3&&maze[i+2][j].state==0)
            {
                build_maze_stack.push(temp);
                maze[i+2][j].state=1;
                maze[i+1][j].state=1;
                i=i+2;
                if(maze[i-1][j].state==0)
                    up=false;
                else
                    up=true;
                if(maze[i+1][j].state==0)
                    down=false;
                else
                    down=true;
                if(maze[i][j-1].state==0)
                    left=false;
                else
                    left=true;
                if(maze[i][j+1].state==0)
                    right=false;
                else
                    right=true;
            }
            else{
                down=true;
            }
            break;
        case 2: if(!left&&j>2&&maze[i][j-2].state==0)
            {
                build_maze_stack.push(temp);
                maze[i][j-2].state=1;
                maze[i][j-1].state=1;
                j=j-2;
                if(maze[i-1][j].state==0)
                    up=false;
                else
                    up=true;
                if(maze[i+1][j].state==0)
                    down=false;
                else
                    down=true;
                if(maze[i][j-1].state==0)
                    left=false;
                else
                    left=true;
                if(maze[i][j+1].state==0)
                    right=false;
                else
                    right=true;
            }
            else{
                left=true;
            }
            break;
        case 3: if(!right&&j<colint-3&&maze[i][j+2].state==0)
            {
                build_maze_stack.push(temp);
                maze[i][j+2].state=1;
                maze[i][j+1].state=1;
                j=j+2;
                if(maze[i-1][j].state==0)
                    up=false;
                else
                    up=true;
                if(maze[i+1][j].state==0)
                    down=false;
                else
                    down=true;
                if(maze[i][j-1].state==0)
                    left=false;
                else
                    left=true;
                if(maze[i][j+1].state==0)
                    right=false;
                else
                    right=true;
            }
            else{
                right=true;
            }
            break;
        }
        if(up&&down&&left&&right){
            if(!build_maze_stack.isEmpty()){
                i=build_maze_stack.top().i;
                j=build_maze_stack.top().j;
                build_maze_stack.pop();
                if(maze[i-1][j].state==0)
                    up=false;
                else
                    up=true;
                if(maze[i+1][j].state==0)
                    down=false;
                else
                    down=true;
                if(maze[i][j-1].state==0)
                    left=false;
                else
                    left=true;
                if(maze[i][j+1].state==0)
                    right=false;
                else
                    right=true;
            }
            else{
                maze[1][1].state=2;
                maze[rowint-3][colint-3].state=3;
                creat_maze=true;
                for(int i=0; i<rowint; i++)//这一段是防止生成迷宫后依旧显示路线
                    for(int j=0; j<colint; j++){
                        path[i][j].state=0;//在这里的状态表示父节点，1,2,3,4分别表示从上下左右发展过来
                        path[i][j].i=i;
                        path[i][j].j=j;
                    }
                //qDebug()<<"creat maze successfully!"<<endl;
                return;
            }
        }
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
            if(i == 0 && j == 0)
                painter.drawPixmap(j*maze_cell_size,i*maze_cell_size,maze_cell_size,maze_cell_size,player);
            else if(maze[i][j].state == 0)
            {
                paint.setBrush(QColor(Qt::black));
            }
            else if(maze[i][j].state == 0)
                paint.setBrush(QColor(Qt::white));
            paint.drawRect(j*maze_cell_size,i*maze_cell_size,maze_cell_size,maze_cell_size);
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


