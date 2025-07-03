#include "barchart.h"
#include <QtCharts/QtCharts>
QChart* BarChart::create(const DataSet& data, bool isGrayscale) {
    auto series = new QBarSeries();
    auto set = new QBarSet("Значения");
    QStringList categories;
    double minY = 0.0, maxY = 0.0;
    bool dataIsNotEmpty = !data.isEmpty();
    if (dataIsNotEmpty) {
        minY = maxY = data.first().second;
        for (const auto& point : data) {
            *set << point.second;
            categories << point.first.toString("dd.MM hh:mm");
            if (point.second < minY) minY = point.second;
            if (point.second > maxY) maxY = point.second;
        }
    }
    if (isGrayscale) {
        QLinearGradient barGradient(0, 0, 0, 300);
        barGradient.setColorAt(0.0, QColor(120, 120, 120));
        barGradient.setColorAt(1.0, QColor(190, 190, 190));
        set->setBrush(barGradient);
        set->setPen(QPen(Qt::black));
    } else {
        set->setColor(QColor(30, 144, 255));
    }
    series->append(set);
    auto chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Столбчатая диаграмма");
    auto axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    auto axisY = new QValueAxis();
    if (dataIsNotEmpty) {
        axisY->setRange(minY < 0 ? minY * 1.1 : 0, maxY * 1.1);
    }
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->hide();
    return chart;
}
QString BarChart::name() const { return "Столбчатая диаграмма"; }
