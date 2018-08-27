#ifndef ADDMATHCHANNELDIALOG_H
#define ADDMATHCHANNELDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include <qwt_plot_curve.h>
#include "PlotJuggler/plotdata.h"
#include "custom_equation.h"

namespace Ui {
class AddMathChannelDialog;
}

class AddMathChannelDialog : public QDialog
{
    Q_OBJECT

    struct SnippetData{
        QString name;
        QString globalVars;
        QString equation;
    };

public:
    explicit AddMathChannelDialog(PlotDataMapRef &plotMapData, QWidget *parent);
    virtual ~AddMathChannelDialog() override;

    void setLinkedPlotName(const QString &linkedPlotName);
    void accept() override;

    QString getLinkedData() const;
    QString getGlobalVars() const;
    QString getEquation() const;
    QString getName() const;
    PlotDataPtr getNewPlotData() const;
    void setFromCustomEquation(CustomEquationPtr data);
    CustomEquationPtr getCustomEquationData() const;

private slots:

    void on_curvesListWidget_doubleClicked(const QModelIndex &index);

    void on_snippetsListWidget_currentRowChanged(int currentRow);

    void on_snippetsListWidget_doubleClicked(const QModelIndex &index);

private:
    static const std::vector<SnippetData> getSnippets();

    PlotDataMapRef &_plotMapData;
    Ui::AddMathChannelDialog *ui;

    PlotDataPtr _newPlotData;
    CustomEquationPtr _customEquation;
};

#endif // ADDMATHCHANNELDIALOG_H
