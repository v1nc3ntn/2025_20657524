#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect button 1 signal to slot
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);

    // Connect button 2 signal to slot
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Connect statusUpdateMessage signal to statusbar's showMessage slot
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
    QMessageBox msgBox;
    msgBox.setText("Button 1 was clicked");
    msgBox.exec();
}

void MainWindow::handleButton2()
{
    emit statusUpdateMessage(QString("Button 2 was clicked"), 0);
    QMessageBox msgBox;
    msgBox.setText("Button 2 was clicked");
    msgBox.exec();
}
