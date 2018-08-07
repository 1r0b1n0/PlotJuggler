#pragma once

#include <string>
#include <QString>

class PlotData;

class CustomEquation
{
public:
    CustomEquation();

private:
    std::string _linkedPlotData;
    QString _globalVars;
    QString _calcEquation;
};
