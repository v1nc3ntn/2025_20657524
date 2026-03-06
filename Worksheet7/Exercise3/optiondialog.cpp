#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

/* Load values from a ModelPart into the dialog fields */
void OptionDialog::loadFromModelPart(ModelPart* part)
{
    ui->lineEdit->setText(part->getName());
    ui->spinBoxR->setValue(part->getColourR());
    ui->spinBoxG->setValue(part->getColourG());
    ui->spinBoxB->setValue(part->getColourB());
    ui->checkBoxVisible->setChecked(part->isVisible());
}

/* Save values from the dialog fields back into a ModelPart */
void OptionDialog::saveToModelPart(ModelPart* part)
{
    part->setName(ui->lineEdit->text());
    part->setColour(
        ui->spinBoxR->value(),
        ui->spinBoxG->value(),
        ui->spinBoxB->value()
    );
    part->setVisible(ui->checkBoxVisible->isChecked());
}