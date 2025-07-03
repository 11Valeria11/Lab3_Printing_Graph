#ifndef PIECHART_H
#define PIECHART_H
#include "interfaces.h"
class PieChart : public IChart {
public:
    QtCharts::QChart* create(const DataSet& data, bool isGrayscale) override;
    QString name() const override;
};
#endif // PIECHART_H
