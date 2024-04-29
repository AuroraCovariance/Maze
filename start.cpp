#include "start.h"
#include "ui_start.h"
#include "mainwindow.h"

QString col;
QString row;

start::start(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::start)
{
    ui->setupUi(this);
    this->setWindowTitle("请设定你的参数");
}

start::~start()
{
    delete ui;
}

void start::on_check_clicked()
{
    col = ui->col->text();
    row = ui->row->text();
    if(0<col.toInt() && col.toInt()<=50 && 0<row.toInt() && row.toInt()<=50){
        MainWindow* w = new MainWindow();
        w->show();
        this->close();
    }
    else if(col.isEmpty() || row.isEmpty())
        QMessageBox::warning(this,tr("输入错误"),tr("请输入"),QMessageBox::Ok);
    else{
        QMessageBox::warning(this,tr("输入错误"),tr("行列数必须小于等于50"),QMessageBox::Ok);
        ui->row->clear();
        ui->col->clear();
    }
}

