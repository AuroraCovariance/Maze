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
    painter.drawPixmap(player_y*maze_cell_size,player_x*maze_cell_size,maze_cell_size,maze_cell_size,player);
    painter.drawPixmap(end.y*maze_cell_size,end.x*maze_cell_size,maze_cell_size,maze_cell_size,coin);

    //绘制bfs搜寻到的路径，用画笔连线起来
    if(autofind_on){
        path = AutoFind(player_x,player_y);
        QPen pen;
        pen.setColor(Qt::yellow);
        pen.setWidth(10);
        paint.setPen(pen);
        for (int i = 0; i < path.size() - 1; ++i) {
            QPoint current = path[i];
            QPoint next = path[i + 1];
            painter.drawLine(current.x() * maze_cell_size + maze_cell_size / 2, current.y() * maze_cell_size + maze_cell_size / 2,
                             next.x() * maze_cell_size + maze_cell_size / 2, next.y() * maze_cell_size + maze_cell_size / 2);
        }
    }

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
        if(maze[player_x+1][player_y] != WALL && player_x+1 <= colint)
            player_x++;
        break;
    case Qt::Key_D:
        if(maze[player_x][player_y+1] != WALL && player_x+1 <= rowint)
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
bool MainWindow::isValid(int x, int y) {
    return (x >= 0 && x < rowint && y >= 0 && y < colint);
}

QVector<QPoint> MainWindow::AutoFind( int startx, int starty) {
    std::queue<std::pair<int, int>> q;
    bool visited[50][50] = {false};
    std::pair<int, int> start = std::make_pair(startx, starty);
    q.push(start);
    visited[startx][starty] = true;

    int dx[] = {1, -1, 0, 0}; // 四个可能得移动方向
    int dy[] = {0, 0, 1, -1};
    QVector<QPoint> path;

    while (!q.empty()) {
        std::pair<int, int> current = q.front();
        q.pop();
        int x = current.first;
        int y = current.second;

        // 找到终点
        if (x == end.x && y == end.y) {
            // 重新构建路径
            while (!(x == startx && y == starty)) {
                path.prepend(QPoint(x, y));
                int prevX = visited[x][y] / 100;
                y = visited[x][y] % 100;
                x = prevX;
            }
            path.prepend(QPoint(startx, starty));
            return path;
        }

        // 访问临近格子
        for (int i = 0; i < 4; ++i) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            // 检查是否访问过
            if (isValid(newX, newY) && maze[newX][newY] == 0 && !visited[newX][newY]) {
                q.push(std::make_pair(newX, newY));
                visited[newX][newY] = x * 100 + y;
            }
        }
    }

    // 没找到
    return QVector<QPoint>();
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


