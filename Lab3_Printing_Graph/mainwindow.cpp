#include "mainwindow.h"
#include <QtWidgets>
#include <QPageSize>
#include "ioc_container.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
}

MainWindow::~MainWindow(){}


void MainWindow::setupUi()
{
    setWindowTitle("Печать графиков (IoC Singleton)");
    setGeometry(100, 100, 1400, 800);

    m_dirModel = new QFileSystemModel(this);
    m_dirModel->setFilter(QDir::AllDirs | QDir::NoDotAndDotDot);
    m_dirModel->setRootPath("");

    m_dirTreeView = new QTreeView(this);
    m_dirTreeView->setModel(m_dirModel);
    m_dirTreeView->setColumnHidden(1, true);
    m_dirTreeView->setColumnHidden(2, true);
    m_dirTreeView->setColumnHidden(3, true);
    m_dirTreeView->header()->setSectionResizeMode(0, QHeaderView::Stretch);

    m_fileModel = new QFileSystemModel(this);
    m_fileModel->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    m_fileModel->setNameFilters({"*.sqlite", "*.json"});
    m_fileModel->setNameFilterDisables(false);
    m_fileModel->setRootPath(QDir::homePath());

    m_fileView = new QTableView(this);
    m_fileView->setModel(m_fileModel);
    m_fileView->setRootIndex(m_fileModel->index(QDir::homePath()));
    m_fileView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_fileView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto fileBrowserSplitter = new QSplitter(Qt::Horizontal, this);
    fileBrowserSplitter->addWidget(m_dirTreeView);
    fileBrowserSplitter->addWidget(m_fileView);
    fileBrowserSplitter->setStretchFactor(0, 1);
    fileBrowserSplitter->setStretchFactor(1, 2);

    auto rightWidget = new QWidget(this);
    auto rightLayout = new QVBoxLayout(rightWidget);

    auto controlPanel = new QHBoxLayout();
    m_chartTypeCombo = new QComboBox(this);

    // Динамически заполняем ComboBox
    m_chartTypeCombo->addItem(IoCContainer::instance().resolve<IChart>("line_chart")->name());
    m_chartTypeCombo->addItem(IoCContainer::instance().resolve<IChart>("bar_chart")->name());
    m_chartTypeCombo->addItem(IoCContainer::instance().resolve<IChart>("pie_chart")->name());

    m_grayscaleCheck = new QCheckBox("Черно-белый график", this);
    auto printButton = new QPushButton("Сохранить в PDF", this);

    controlPanel->addWidget(new QLabel("Тип диаграммы:", this));
    controlPanel->addWidget(m_chartTypeCombo);
    controlPanel->addWidget(m_grayscaleCheck);
    controlPanel->addStretch();
    controlPanel->addWidget(printButton);

    m_chartView = new QChartView(this);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    updateChart();

    rightLayout->addLayout(controlPanel);
    rightLayout->addWidget(m_chartView);

    auto mainSplitter = new QSplitter(this);
    mainSplitter->addWidget(fileBrowserSplitter);
    mainSplitter->addWidget(rightWidget);
    mainSplitter->setStretchFactor(0, 2);
    mainSplitter->setStretchFactor(1, 3);
    setCentralWidget(mainSplitter);

    connect(m_dirTreeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onDirectorySelected);
    connect(m_fileView, &QTableView::doubleClicked, this, &MainWindow::onFileDoubleClicked);
    connect(m_chartTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::updateChart);
    connect(m_grayscaleCheck, &QCheckBox::stateChanged, this, &MainWindow::updateChart);
    connect(printButton, &QPushButton::clicked, this, &MainWindow::onPrintChart);
}



void MainWindow::onDirectorySelected(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    const QModelIndex index = m_dirTreeView->selectionModel()->currentIndex();
    if (!index.isValid()) return;
    const QString path = m_dirModel->filePath(index);
    m_fileView->setRootIndex(m_fileModel->setRootPath(path));
}

void MainWindow::onFileDoubleClicked(const QModelIndex &index)
{
    const QString filePath = m_fileModel->filePath(index);
    QFileInfo fileInfo(filePath);
    m_currentData.clear();
    try {
        if (fileInfo.suffix().toLower() == "sqlite") {
            auto reader = IoCContainer::instance().resolve<IDataReader>("sqlite_reader");
            m_currentData = reader->read(filePath);
        } else if (fileInfo.suffix().toLower() == "json") {
            auto reader = IoCContainer::instance().resolve<IDataReader>("json_reader");
            m_currentData = reader->read(filePath);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при чтении файла: %1").arg(e.what()));
    }
    if(m_currentData.isEmpty()) {
        QMessageBox::warning(this, "Ошибка чтения", "Не удалось прочитать данные из файла или файл пуст.");
    }
    updateChart();
}

void MainWindow::updateChart()
{
    if (m_currentData.isEmpty()) {
        auto chart = new QChart();
        chart->setTitle("Выберите файл для отображения данных");
        m_chartView->setChart(chart);
        return;
    }
    try {
        QString currentChartName = m_chartTypeCombo->currentText();
        std::shared_ptr<IChart> factory;
        if (currentChartName == "Линейный график") {
            factory = IoCContainer::instance().resolve<IChart>("line_chart");
        } else if (currentChartName == "Столбчатая диаграмма") {
            factory = IoCContainer::instance().resolve<IChart>("bar_chart");
        } else if (currentChartName == "Круговая диаграмма") {
            factory = IoCContainer::instance().resolve<IChart>("pie_chart");
        }
        if (factory) {
            bool isGrayscale = m_grayscaleCheck->isChecked();
            auto chart = factory->create(m_currentData, isGrayscale);
            m_chartView->setChart(chart);
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Ошибка", QString("Ошибка при создании графика: %1").arg(e.what()));
    }
}

void MainWindow::onPrintChart()
{
    if (!m_chartView->chart() || m_currentData.isEmpty()) {
        QMessageBox::information(this, "Сохранение", "График не отображен. Нечего сохранять.");
        return;
    }
    QString filePath = QFileDialog::getSaveFileName(this, "Сохранить график как PDF", "", "PDF Files (*.pdf)");
    if (filePath.isEmpty()) return;
    QPdfWriter writer(filePath);
    writer.setCreator("ChartApp");
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);
    m_chartView->render(&painter);
    painter.end();
    QMessageBox::information(this, "Сохранение", "График успешно сохранен в " + filePath);
}

