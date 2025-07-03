#include "mainwindow.h"
#include <QApplication>
#include <memory>
#include <QList>

#include "ioc_container.h"
#include "interfaces.h"
#include "sqlitereader.h"
#include "jsonreader.h"
#include "linechart.h"
#include "barchart.h"
#include "piechart.h"

void registerServices()
{
    // Получаем доступ к единственному экземпляру контейнера
    auto& ioc = IoCContainer::instance();

    // Регистрируем ридеры
    ioc.registerService<IDataReader>("sqlite_reader", std::make_shared<SqliteReader>());
    ioc.registerService<IDataReader>("json_reader", std::make_shared<JsonReader>());

    // Регистрируем классы для создания графиков
    ioc.registerService<IChart>("line_chart", std::make_shared<LineChart>());
    ioc.registerService<IChart>("bar_chart", std::make_shared<BarChart>());
    ioc.registerService<IChart>("pie_chart", std::make_shared<PieChart>());
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    registerServices();
    MainWindow w;
    w.show();

    return a.exec();
}
