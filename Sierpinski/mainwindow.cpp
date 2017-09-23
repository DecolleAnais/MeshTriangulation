#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, SIGNAL(released()), this, SLOT(onButton()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onButton() {
    ui->widget->changeDisplayMode();
}
