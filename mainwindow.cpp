#include "mainwindow.h"
#include "./ui_mainwindow.h"
#define WALL -1
#define NOTHING 2
#define down 1
#define right 2
#define left 4
#define up 8
#define DESTINATION 6

extern QString col;
extern QString row;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("迷宫");
    player.load(":/player.jpg");
    coin.load(":/coin.jpg");
    colint = col.toInt();
    rowint = row.toInt();
    generate();
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

//使用prim算法生成迷宫
void MainWindow::generate()
{
    memset(maze,WALL,sizeof(maze));
    //定义起始点
    maze[0][0] = NOTHING;
    start.x = start.y = 0;
    player_x = player_y = 0;
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

    do{
    end.x = rand() % rowint;
    end.y = rand() % colint;
    }while(maze[end.x][end.y]  != NOTHING);
    maze[end.x][end.y] = DESTINATION;
    //随机得到终点

}

//绘制界面
void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    QPainter paint(this);
    int max;
    if(rowint>=colint)
        max=rowint;
    else
        max=colint;

    maze_cell_size=600/max;//600是为了保证不超出界面，用650有时会超出（调了好久）
    for(int i=0;i<colint;++i)
    {
        for(int j=0;j<rowint;++j)
        {
            if(maze[i][j] == NOTHING)
            {
                paint.setBrush(QColor(Qt::black));
            }
            else if(maze[i][j] == WALL)
                paint.setBrush(QColor(Qt::white));
            paint.drawRect(j*maze_cell_size,i*maze_cell_size,maze_cell_size,maze_cell_size);
        }
    }
    //绘制迷宫

    //绘制bfs搜寻到的路径，用画笔连线起来
    if(autofind_on){
        AutoFind(player_x,player_y);
        QPen pen;
        pen.setColor(Qt::yellow);
        pen.setWidth(10);
        paint.setPen(pen);
        for(int i=0;i<m_vector.size();++i)
        {
            if(i!=m_vector.size()-1)
            {
                QPointF aa(m_vector[i].y()*maze_cell_size+maze_cell_size/2.0,m_vector[i].x()*maze_cell_size+maze_cell_size/2.0);
                QPointF bb(m_vector[i+1].y()*maze_cell_size+maze_cell_size/2.0,m_vector[i+1].x()*maze_cell_size+maze_cell_size/2.0);
                paint.drawLine(aa,bb);
            }

        }
    }

    painter.drawPixmap(player_y*maze_cell_size,player_x*maze_cell_size,maze_cell_size,maze_cell_size,player);
    painter.drawPixmap(end.y*maze_cell_size,end.x*maze_cell_size,maze_cell_size,maze_cell_size,coin);
    //绘制角色和金币

    update();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_W:
        if(maze[player_x-1][player_y] != WALL && player_x > 0)
            player_x--;
        break;
    case Qt::Key_A:
        if(maze[player_x][player_y-1] != WALL && player_y > 0)
            player_y--;
        break;
    case Qt::Key_S:
        if(maze[player_x+1][player_y] != WALL && player_x+1 < colint)
            player_x++;
        break;
    case Qt::Key_D:
        if(maze[player_x][player_y+1] != WALL && player_x+1 < rowint-2)
            player_y++;
        break;
    case Qt::Key_U:
        autofind_on = !autofind_on;
        break;
    default:
        break;
    }
    if(maze[player_x][player_y] == DESTINATION)
        StopGame();
}

//使用bfs找到路径

void MainWindow::AutoFind(int startx ,int starty) {

    m_vector.clear();
    std::list<node*>saveNode;  //用来保存new的node节点，后面统一释放，避免内存泄漏
    node *a=new node(QPoint(startx,starty));visited[startx][starty]=1;
    a->parent=NULL;
    saveNode.push_back(a);
    std::queue<node*>tempQueue;
    tempQueue.push(a);
    while(!tempQueue.empty())
    {
        node *aa=tempQueue.front();
        tempQueue.pop();
        int _x=aa->m_point.x();
        int _y=aa->m_point.y();


        QList<QPoint>tempList;
        //将上下左右四个方向的位置添加到待访问位置中
        if((_x-1>=0) && maze[_x-1][_y]!=WALL && visited[_x-1][_y]==0)
            tempList.append(QPoint(_x-1,_y));
        if((_x+1)<=rowint && maze[_x+1][_y]!=WALL && visited[_x+1][_y]==0)
            tempList.append(QPoint(_x+1,_y));
        if((_y-1)>=0 && maze[_x][_y-1]!=WALL && visited[_x][_y-1]==0)
            tempList.append(QPoint(_x,_y-1));
        if((_y+1)<=colint && maze[_x][_y+1]!=WALL  && visited[_x][_y+1]==0)
            tempList.append(QPoint(_x,_y+1));
        while(!tempList.empty())
        {
            //随机取出上下左右四个位置中的一个
            int x11=rand()%(tempList.size());
            int _x1=tempList.at(x11).x();
            int _y1=tempList.at(x11).y();
            if(_x1==end.x && _y1==end.y)  //找到路径
            {

                node *tempaa=aa;
                while(tempaa)
                {
                    //  将搜寻到的路径点保存到vector中
                    m_vector.push_front(QPoint(tempaa->m_point.x(),tempaa->m_point.y()));
                    tempaa=tempaa->parent;
                }

                //删除所有Node指针
                for(auto it:saveNode)
                {
                    if(it !=NULL)
                    {
                        delete it;
                        it=NULL;
                    }
                }
                return ;
            }

            visited[_x1][_y1]=1;
            node *a1=new node(QPoint(_x1,_y1));
            saveNode.push_back(a1);
            a1->parent=aa;
            tempQueue.push(a1);
            tempList.removeAt(x11);

        }

    }
}

void MainWindow::StopGame()
{
    QMessageBox::information(this,tr("win"),tr("你赢了"),QMessageBox::Ok);
    this->close();
}

MainWindow::~MainWindow()
{
    delete ui;
}


