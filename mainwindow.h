#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPainter>
#include<QPixmap>
#include<vector>
#include<ctime>

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
    void FindBlock();
    void generate();
    ~MainWindow();

private:
    int maze[50][50];
    int colint;
    int rowint;
    int maze_cell_size;

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

    QPixmap player;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
