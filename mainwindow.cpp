 #include "mainwindow.h"
#include "ui_mainwindow.h"




std::string pitchChar[37] = { "0", "Low Do ", "Low Do#", "Low Re ", "Low Re#", "Low Mi", "Low Fa", "Low Fa#", "Low So", "Low So#", "Low La", "Low La#", "Low si","Do", "Do#", "Re", "Re#", "Mi", "Fa", "Fa#", "So", "So#", "La", "La#", "si","High Do", "High Do#", "High Re", "High Re#", "High Mi", "High Fa", "High Fa#", "High So", "High So#", "High La", "High La#", "High si" };



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    this ->hide();
    offlinePage->show();
    offlinePage->exec();
    this->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_3_clicked()
{
    this ->hide();
    onlinePage->show();
    onlinePage->exec();
    this->show();
}
