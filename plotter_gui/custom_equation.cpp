#include <custom_equation.h>
#include <QJSEngine>
#include <QFile>
#include <add_math_channel.h>
#include <QMessageBox>

CustomEquation::CustomEquation(const std::string &linkedPlot, const std::string &plotName, const QString &globalVars, const QString &equation):
    _linkedPlot(linkedPlot),
    _plotName(plotName),
    _globalVars(globalVars),
    _calcEquation(equation)
{

}

/*
CustomEquationPtr CustomEquation::createNew(const std::string &linkedPlot, PlotDataMapRef &plotData, QWidget *parent)
{

    AddMathChannelDialog dialog(plotData, parent);
    dialog.setLinkedPlotName(QString::fromStdString(linkedPlot));
    if(dialog.exec() == QDialog::Accepted)
    {
        try {
            CustomEquationPtr ret;
            ret.reset(new CustomEquation(linkedPlot, dialog.getName().toStdString(), dialog.getGlobalVars(), dialog.getEquation()));
            ret->recalc(plotData);

            return ret;
        } catch (std::runtime_error e) {
            QMessageBox::critical(parent, "Error", "Failed to create custom equation : " + QString::fromStdString(e.what()));
        }
    }

    return CustomEquationPtr();
}
*/

void CustomEquation::calc(PlotDataMapRef &plotData, PlotData &out)
{
    QJSEngine jsEngine;

    {
        QFile file("plotter_gui/resources/common.js");
        //QFile file(":/js/resources/common.js")
        file.open(QIODevice::ReadOnly);
        QString commonData = QString::fromUtf8(file.readAll());
        jsEngine.evaluate(commonData);
    }

    QList<PlotData*> usedChannels;
    QString mathEquationReplaced = _calcEquation;
    while(true)
    {
        int pos1=mathEquationReplaced.indexOf("$$");
        if(pos1 == -1)
            break;

        int pos2 = mathEquationReplaced.indexOf("$$", pos1+2);
        if(pos2 == -1)
        {
            throw std::runtime_error("syntax error : invalid use of $$ macro");
        }

        QString channelName = mathEquationReplaced.mid(pos1+2, pos2-pos1-2);
        QString jsExpression = QString("CHANNEL_VALUES[%1]").arg(usedChannels.size());
        mathEquationReplaced.replace(QStringLiteral("$$%1$$").arg(channelName), jsExpression);

        auto plotDataIt = plotData.numeric.find(channelName.toStdString());
        if(plotDataIt == plotData.numeric.end())
        {
            throw std::runtime_error("invalid channel");
        }

        usedChannels.push_back(&plotDataIt->second);
    }
    //qDebug() << "final equation string : " << mathEquationReplaced;
    jsEngine.evaluate(_globalVars);
    QString calcMethodStr = QString("function calc(x, y, CHANNEL_VALUES){with (Math){%1}}").arg(mathEquationReplaced);
    jsEngine.evaluate(calcMethodStr);
    QJSValue calcFct = jsEngine.evaluate("calc");

    PlotData *oldPlotData;
    auto oldPlotDataIt = plotData.numeric.find(_linkedPlot);
    if(oldPlotDataIt == plotData.numeric.end())
    {
        throw std::runtime_error("invalid linked data channel");
    }
    oldPlotData = &oldPlotDataIt->second;

    out.clear();
    for(size_t i=0;i<oldPlotData->size();++i)
    {
        const PlotData::Point &oldPoint = oldPlotData->at(i);

        QJSValue channelValues = jsEngine.newArray(static_cast<quint32>(usedChannels.size()));
        for(int channelIndex = 0; channelIndex<usedChannels.size(); ++channelIndex)
        {
            PlotData *channelData = usedChannels[channelIndex];
            double value;
            int index = channelData->getIndexFromX(oldPoint.x);
            if(index != -1)
                value = channelData->at(static_cast<size_t>(index)).y;
            else
                value = std::numeric_limits<double>::quiet_NaN();

            channelValues.setProperty(static_cast<quint32>(channelIndex), QJSValue(value));
        }

        PlotData::Point newPoint;
        newPoint.x = oldPoint.x;
        //jsEngine.globalObject().setProperty("CHANNEL_VALUES", channelValues);
        QJSValue jsData = calcFct.call({QJSValue(oldPoint.x), QJSValue(oldPoint.y), channelValues});
        if(jsData.isError())
        {
            throw std::runtime_error("JS Engine : " + jsData.toString().toStdString());
        }
        newPoint.y = jsData.toNumber();

        out.pushBack(newPoint);
    }
}

const std::string &CustomEquation::getName()
{
    return _plotName;
}

const std::string &CustomEquation::getLinkedPlot()
{
    return _linkedPlot;
}

const QString &CustomEquation::getGlobalVars()
{
    return _globalVars;
}

const QString &CustomEquation::getEquation()
{
    return _calcEquation;
}
