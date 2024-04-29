#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QPixmap>
#include<vector>
#include<ctime>
#include<QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
    void generate();
    ~MainWindow();

private:
    struct point{
    public:
        int i;
        int j;//行列坐标
        int state;//对应不同状态：不可行走，可行走,人在上面，终点或者以后可以扩充的状态
        //不可行走-0；可行走-1；人-2；终点-3
    };

    point maze[50][50];
    int colint;
    int rowint;
    int maze_cell_size;
    QStack<point> build_maze_stack;//创建迷宫的栈
    point path[200][200];//用于搜索路径的矩阵
    bool creat_maze=false;

    QPixmap player;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
