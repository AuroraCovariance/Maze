#ifndef START_H
#define START_H

#include <QWidget>
#include "ui_start.h"
#include<QMessageBox>


namespace Ui {
class start;
}

class start : public QWidget
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = nullptr);
    ~start();

private:
    Ui::start *ui;

private slots:
    void on_check_clicked();
};

#endif // START_H
