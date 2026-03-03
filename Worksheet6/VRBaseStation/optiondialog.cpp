#include "optiondialog.h"
#include "ui_optiondialog.h"
#include <QString>

OptionDialog::OptionDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

QString OptionDialog::getText() const
{
    return ui->lineEdit->text();
} 

OptionDialog::~OptionDialog()
{
    delete ui;
}