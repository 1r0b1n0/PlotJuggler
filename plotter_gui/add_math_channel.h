#ifndef ADDMATHCHANNELDIALOG_H
#define ADDMATHCHANNELDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <qwt_plot_curve.h>
#include "PlotJuggler/plotdata.h"

namespace Ui {
class AddMathChannelDialog;
}

class AddMathChannelDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddMathChannelDialog(PlotDataMapRef &plotData, QWidget *parent);
    ~AddMathChannelDialog();
    QString getLinkedData() const;
    QString getGlobalVars() const;
    QString getEquation() const;
    QString getName() const;

private slots:

private:
    PlotDataMapRef &_plotData;
    Ui::AddMathChannelDialog *ui;
};

#endif // ADDMATHCHANNELDIALOG_H
