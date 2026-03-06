#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include "ModelPart.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OptionDialog; }
QT_END_NAMESPACE

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget* parent = nullptr);
    ~OptionDialog();

    /* Load values FROM a ModelPart INTO the dialog */
    void loadFromModelPart(ModelPart* part);

    /* Save values FROM the dialog INTO a ModelPart */
    void saveToModelPart(ModelPart* part);

private:
    Ui::OptionDialog* ui;
};

#endif // OPTIONDIALOG_H