#ifndef BARCHART_H
#define BARCHART_H
#include "interfaces.h"
class BarChart : public IChart {
public:
    QtCharts::QChart* create(const DataSet& data, bool isGrayscale) override;
    QString name() const override;
};
#endif // BARCHART_H
