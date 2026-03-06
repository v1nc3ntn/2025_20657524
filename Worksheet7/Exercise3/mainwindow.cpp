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

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Add context menu action to treeView
    ui->treeView->addAction(ui->actionItem_Options);
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

    /* Link a render window with the Qt widget */
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    /* Add a renderer */
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    /* Create an object and add to renderer */
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

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
    // Update the selected tree item's name to the filename
    QModelIndex index = ui->treeView->currentIndex();
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        selectedPart->setName(fileName);
        emit statusUpdateMessage(QString("Opened: ") + fileName, 0);
    }
    else {
        emit statusUpdateMessage(QString("No item selected - please select a tree item first"), 0);
    }
}

void MainWindow::on_actionItem_Options_triggered()
{
    // Get the selected item
    QModelIndex index = ui->treeView->currentIndex();
    if (!index.isValid()) {
        emit statusUpdateMessage(QString("No item selected"), 0);
        return;
    }
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
    // Create dialog and load current values from the selected part
    OptionDialog dialog(this);
    dialog.loadFromModelPart(selectedPart);
    // Show dialog and save values back if accepted
    if (dialog.exec() == QDialog::Accepted) {
        dialog.saveToModelPart(selectedPart);
        emit statusUpdateMessage(QString("Item updated: ") + selectedPart->getName(), 0);
    }
    else {
        emit statusUpdateMessage(QString("Edit cancelled"), 0);
    }
}