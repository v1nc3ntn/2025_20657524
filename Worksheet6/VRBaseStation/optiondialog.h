#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class OptionDialog; }
QT_END_NAMESPACE

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget* parent = nullptr);
    ~OptionDialog();

    QString getText() const;

private:
    Ui::OptionDialog* ui;
};



#endif // OPTIONDIALOG_H