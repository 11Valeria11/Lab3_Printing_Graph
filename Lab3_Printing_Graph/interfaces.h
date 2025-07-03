#ifndef INTERFACES_H
#define INTERFACES_H

#include <QtCharts/QtCharts>
#include <QList>
#include <QPair>
#include <QDateTime>
#include <QString>

QT_CHARTS_USE_NAMESPACE

    using DataPoint = QPair<QDateTime, double>;
using DataSet = QList<DataPoint>;

class IDataReader {
public:
    virtual ~IDataReader() = default;
    virtual DataSet read(const QString& filePath) = 0;
};

class IChart {
public:
    virtual ~IChart() = default;
    virtual QChart* create(const DataSet& data, bool isGrayscale) = 0;
    virtual QString name() const = 0;
};

#endif // INTERFACES_H
