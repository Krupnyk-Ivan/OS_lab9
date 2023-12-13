#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "Server.h"

std::vector<QString> vec;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Server server;
    server.start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    dial = new Dialog(this);
    dial->show();
}


void MainWindow::on_pushButton_2_clicked()
{
    ui->listWidget->clear();
    if(vec.size()!=0){
        for(int i=0;i<vec.size();i++){
        QString st = vec[i];
        //QMessageBox::information(this,"fg",st);
        ui->listWidget->addItem(st);
        }
    }
}

