#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "optiondialog.h"
#include <QFileDialog>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItem_Options);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(this, &MainWindow::statusUpdateMessage,
        ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);

    this->partList = new ModelPartList("Parts List");
    ui->treeView->setModel(this->partList);

    ModelPart* rootItem = this->partList->getRootItem();
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

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Reset Camera */
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
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
    emit statusUpdateMessage(QString("Button 2 was clicked"), 0);
}

void MainWindow::handleTreeClicked()
{
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
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

    QModelIndex index = ui->treeView->currentIndex();

    QStringList fileInfo = fileName.split("/");
    if (fileInfo.isEmpty())
        fileInfo = fileName.split("\\");
    QString name = fileInfo.last();

    ModelPart* newItem = new ModelPart({ name, QString("true") });

    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        selectedPart->appendChild(newItem);
    }
    else {
        partList->getRootItem()->appendChild(newItem);
    }

    newItem->loadSTL(fileName);

    partList->layoutChanged();
    updateRender();

    emit statusUpdateMessage(QString("Opened: ") + fileName, 0);
}

void MainWindow::on_actionItem_Options_triggered()
{
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected"), 0);
        return;
    }
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    OptionDialog dialog(this);
    dialog.loadFromModelPart(selectedPart);
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToModelPart(selectedPart);
        emit statusUpdateMessage(QString("Item updated: ") + selectedPart->getName(), 0);
    }
    else {
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}

void MainWindow::updateRender()
{
    renderer->RemoveAllViewProps();
    int rows = partList->rowCount(QModelIndex());
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }
    renderer->ResetCamera();
    renderer->Render();
    renderWindow->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index)
{
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor) {
            renderer->AddActor(actor);
        }
    }

    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }

    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}