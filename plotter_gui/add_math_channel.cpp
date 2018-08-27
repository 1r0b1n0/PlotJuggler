#include "add_math_channel.h"
#include "ui_add_math_channel.h"
#include <QDebug>
#include "plotwidget.h"

AddMathChannelDialog::AddMathChannelDialog(PlotDataMapRef &plotData, QWidget *parent) :
    QDialog(parent),
    _plotData(plotData),
    ui(new Ui::AddMathChannelDialog)
{
    ui->setupUi(this);

    for(const auto &p : plotData.numeric)
    {
        QString label = QString::fromStdString(p.first);
        ui->linkedChannelCombobox->addItem(label);
    }
}

AddMathChannelDialog::~AddMathChannelDialog()
{
    delete ui;
}

QString AddMathChannelDialog::getLinkedData() const
{
    return ui->linkedChannelCombobox->currentText();
}

QString AddMathChannelDialog::getGlobalVars() const
{
    return ui->globalVarsTextField->toPlainText();
}

QString AddMathChannelDialog::getEquation() const
{
    return ui->mathEquation->toPlainText();
}

QString AddMathChannelDialog::getName() const
{
    return ui->nameLineEdit->text();
}


