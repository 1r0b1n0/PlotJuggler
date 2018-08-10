#ifndef PLOTDATAMODEL_H
#define PLOTDATAMODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include <unordered_map>
#include <string>
#include <vector>
#include "plotdata.h"

class PlotDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    PlotDataModel(QObject *parent=nullptr);

    void ResetData(const PlotDataMap &map);

    PlotData& GetNumericPlotDataAtRow(int row);
    const std::unordered_map<std::string, PlotDataPtr> &getNumericData() const;
    const std::unordered_map<std::string, PlotDataAnyPtr> &getUserDefinedData() const;

public slots:

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:

    // index based vector, with copy of the name in a QString instead of a std::string
    QList<std::pair<QString, PlotDataPtr>> _numeric_vector;

    std::unordered_map<std::string, PlotDataPtr>     _numeric;
    std::unordered_map<std::string, PlotDataAnyPtr>  _user_defined;
};

#endif // PLOTDATAMODEL_H
