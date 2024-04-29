#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QPen>
#include<QPixmap>
#include<vector>
#include<ctime>
#include<QKeyEvent>
#include<QMessageBox>
#include<queue>
#include <utility>
#include <QVector>
#include <QPoint>

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
    void keyPressEvent(QKeyEvent *event);
    void paintEvent(QPaintEvent *event);
    void FindBlock();
    void generate();
    bool isValid(int x, int y);
    QVector<QPoint> AutoFind(int startx, int starty);
    void StopGame();
    ~MainWindow();

private:
    int maze[50][50];
    int colint;
    int rowint;
    int maze_cell_size;
    int player_x,player_y;
    bool autofind_on;

    struct block{
        int row,column,direction;
        block(int _row,int _column,int _direction){
            row = _row;
            column = _column;
            direction = _direction;
        }
    };
    struct point {
        int x;
        int y;
    }start,end;
    std::vector<block> myblock;
    int x_num=0,y_num=0;

    struct node  //宽度搜索时，用来获取到每一次访问的路径
    {
        public:
            node(QPoint a):m_point(a),parent(nullptr)
            {

            }
            QPoint m_point;
            node* parent;
    };
    int _maze[50][50] = {0};
    QVector<QPoint>m_vector;
    QVector<QPoint>path;

    QPixmap player;
    QPixmap coin;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
