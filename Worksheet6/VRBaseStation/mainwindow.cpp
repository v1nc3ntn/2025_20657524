#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "optiondialog.h"
#include <QDialog>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect button signals to slots
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);

    // Connect statusUpdateMessage signal to statusbar
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);

    // Connect treeView click signal to slot
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    // Create ModelPartList and link to treeView
    this->partList = new ModelPartList("Parts List");
    ui->treeView->setModel(this->partList);

    // Get root item
    ModelPart* rootItem = this->partList->getRootItem();

    // Add 3 top level items each with 5 children
    for (int i = 0; i < 3; i++) {
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

        ModelPart* childItem = new ModelPart({ name, visible });
        rootItem->appendChild(childItem);

        for (int j = 0; j < 5; j++) {
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");

            ModelPart* childChildItem = new ModelPart({ name, visible });
            childItem->appendChild(childChildItem);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1()
{
    emit statusUpdateMessage(QString("Button 1 was clicked"), 0);
}

void MainWindow::handleButton2()
{
    OptionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        emit statusUpdateMessage(QString("Dialog accepted ") + dialog.getText(), 0);
    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected"), 0);
    }
    
}

void MainWindow::handleTreeClicked()
{
    // Get the index of the selected item
    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the item from the index
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Retrieve the name string from the item
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);
}

void MainWindow::on_actionOpen_File_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files (*.stl);;Text Files (*.txt)")
    );

    if (fileName.isEmpty())
        return;

    emit statusUpdateMessage(QString("Select File: ") + fileName, 0);
}