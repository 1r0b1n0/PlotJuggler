#include "PlotJuggler/PlotDataModel.h"
#include <stdexcept>

PlotDataModel::PlotDataModel(QObject *parent):
    QAbstractTableModel(parent)
{

}

void PlotDataModel::ResetData(const PlotDataMap &map)
{

}

PlotData &PlotDataModel::GetNumericPlotDataAtRow(int row)
{
    if(row >= 0 || row < _numeric_vector.size())
    {
        return *(_numeric_vector[row].second);
    }

    throw std::runtime_error("PlotDataModel::GetNumericPlotDataAtRow() : inavlid row");
}

const std::unordered_map<std::string, PlotDataPtr> &PlotDataModel::getNumericData() const
{
    return _numeric;
}

const std::unordered_map<std::string, PlotDataAnyPtr> &PlotDataModel::getUserDefinedData() const
{
    return _user_defined;
}

QVariant PlotDataModel::data(const QModelIndex &index, int role) const
{
    if(index.parent().isValid())
        return QVariant();

    if(index.row() >= 0 || index.row() < _numeric_vector.size())
    {
        if(role == Qt::DisplayRole)
        {
            if(index.column() == 0)
            {
                QVariant(_numeric_vector[index.row()].first);
            }
        }
    }

    return QVariant();
}

int PlotDataModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return _numeric_vector.size();
}

int PlotDataModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return 0;

    return 2;
}
