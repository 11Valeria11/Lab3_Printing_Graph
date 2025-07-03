#include "piechart.h"
#include <QtCharts/QtCharts>
#include <cmath>
#include <algorithm>
QChart* PieChart::create(const DataSet& data, bool isGrayscale) {
    const int TARGET_SLICE_COUNT = 150;
    DataSet dataToDraw;
    QString title = "Круговая диаграмма";
    if (data.size() > TARGET_SLICE_COUNT) {
        title += QString(" (агрегировано из %1 точек)").arg(data.size());
        DataSet sortedData = data;
        std::sort(sortedData.begin(), sortedData.end(), [](const DataPoint& a, const DataPoint& b) {
            return a.first < b.first;
        });
        int groupSize = static_cast<int>(std::ceil(static_cast<double>(sortedData.size()) / TARGET_SLICE_COUNT));
        for (int i = 0; i < sortedData.size(); i += groupSize) {
            int currentGroupSize = qMin(groupSize, sortedData.size() - i);
            if (currentGroupSize == 0) continue;
            double sum = 0.0;
            for (int j = 0; j < currentGroupSize; ++j) {
                sum += sortedData[i + j].second;
            }
            dataToDraw.append({sortedData[i].first, sum / currentGroupSize});
        }
    } else {
        dataToDraw = data;
    }
    auto series = new QPieSeries();
    for (const auto& point : dataToDraw) {
        series->append(point.first.toString("hh:mm:ss"), point.second);
    }
    series->setLabelsVisible(false);
    const auto slices = series->slices();
    for (int i = 0; i < slices.count(); ++i) {
        QPieSlice *slice = slices.at(i);
        slice->setBorderColor(Qt::black);
        slice->setBorderWidth(1);
        if (isGrayscale) {
            int grayValue = 50 + (180 * i) / qMax(1, slices.count());
            slice->setColor(QColor(grayValue, grayValue, grayValue));
        }
    }
    auto chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(title);
    chart->legend()->hide();
    return chart;
}
QString PieChart::name() const { return "Круговая диаграмма"; }
