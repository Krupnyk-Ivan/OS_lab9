#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include "Client.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    Client client;

    client.connectToServer("127.0.0.1", 1488);
    client.setUsername("Artem");
    client.chooseConnectionMethod(1);

    client.sendMessage("henlo");
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    QString str;
    str = ui->lineEdit->text() + " " + ui->lineEdit_2->text();
    vec.push_back(str);
}

