#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCharts/QtCharts>
#include <QMainWindow>
#include <QFileSystemModel>
#include <memory>
#include "interfaces.h"

class QTreeView;
class QTableView;
class QComboBox;
class QCheckBox;

QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onDirectorySelected(const QItemSelection &selected, const QItemSelection &deselected);
    void onFileDoubleClicked(const QModelIndex &index);
    void updateChart();
    void onPrintChart();

private:
    void setupUi();

    QFileSystemModel* m_dirModel;
    QFileSystemModel* m_fileModel;
    QTreeView*        m_dirTreeView;
    QTableView*       m_fileView;
    QChartView*       m_chartView;
    QComboBox*        m_chartTypeCombo;
    QCheckBox*        m_grayscaleCheck;
    DataSet m_currentData;
};

#endif // MAINWINDOW_H
