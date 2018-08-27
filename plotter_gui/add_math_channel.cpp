#include "add_math_channel.h"
#include "ui_add_math_channel.h"
#include <QDebug>
#include <QMessageBox>
#include "custom_equation.h"
#include "plotwidget.h"

AddMathChannelDialog::AddMathChannelDialog(PlotDataMapRef &plotMapData, QWidget *parent) :
    QDialog(parent),
    _plotMapData(plotMapData),
    ui(new Ui::AddMathChannelDialog)
{
    ui->setupUi(this);

    for(const auto &p : plotMapData.numeric)
    {
        QString label = QString::fromStdString(p.first);
        ui->linkedChannelCombobox->addItem(label);
    }
}

AddMathChannelDialog::~AddMathChannelDialog()
{
    delete ui;
}

void AddMathChannelDialog::setLinkedPlotName(const QString &linkedPlotName)
{
    ui->linkedChannelCombobox->setCurrentText(linkedPlotName);
    ui->linkedChannelCombobox->setEnabled(false);
}

void AddMathChannelDialog::accept()
{
    try {
        std::string plotName = getName().toStdString();
        CustomEquationPtr ce = std::make_shared<CustomEquation>(getLinkedData().toStdString(), plotName, getGlobalVars(), getEquation());
        PlotDataPtr newData = std::make_shared<PlotData>(plotName);
        ce->calc(_plotMapData, *newData);

        _customEquation = ce;
        _newPlotData = newData;
        QDialog::accept();
    }
    catch (std::runtime_error e) {
        _customEquation.reset();
        _newPlotData.reset();

        QMessageBox::critical(this, "Error", "Failed to create custom equation : " + QString::fromStdString(e.what()));
    }
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

PlotDataPtr AddMathChannelDialog::getNewPlotData() const
{
    return _newPlotData;
}

void AddMathChannelDialog::setFromCustomEquation(CustomEquationPtr data)
{
    ui->globalVarsTextField->setPlainText(data->getGlobalVars());
    ui->mathEquation->setPlainText(data->getEquation());
    setLinkedPlotName(QString::fromStdString(data->getLinkedPlot()));
    ui->pushButtonDone->setText("Update");
    ui->nameLineEdit->setText(QString::fromStdString(data->getName()));
    ui->nameLineEdit->setEnabled(false);
}

CustomEquationPtr AddMathChannelDialog::getCustomEquationData() const
{
    return _customEquation;
}


