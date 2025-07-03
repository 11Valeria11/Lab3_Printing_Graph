#include "linechart.h" //
#include <QtCharts/QtCharts>
QChart* LineChart::create(const DataSet& data, bool isGrayscale) {
    auto series = new QLineSeries();
    for (const auto& point : data) {
        series->append(point.first.toMSecsSinceEpoch(), point.second);
    }
    if (isGrayscale) {
        series->setColor(Qt::darkGray);
    } else {
        series->setColor(QColor(30, 144, 255));
    }
    auto chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setTitle("Линейный график");
    auto axisX = new QDateTimeAxis;
    axisX->setFormat("dd.MM.yy hh:mm");
    axisX->setTitleText("Дата");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    auto axisY = new QValueAxis;
    axisY->setTitleText("Значение");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    return chart;
}
QString LineChart::name() const { return "Линейный график"; }
