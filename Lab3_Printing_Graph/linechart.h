#ifndef LINECHART_H
#define LINECHART_H
#include "interfaces.h"
class LineChart : public IChart {
public:
    QtCharts::QChart* create(const DataSet& data, bool isGrayscale) override;
    QString name() const override;
};
#endif // LINECHART_H
