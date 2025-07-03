#include "mainwindow.h"
#include <QtWidgets> // Для Q_UNUSED

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Пока пустой
}

MainWindow::~MainWindow()
{
}

void MainWindow::onDirectorySelected(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
}

void MainWindow::onFileDoubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
}

void MainWindow::updateChart()
{
}

void MainWindow::onPrintChart()
{
}

void MainWindow::setupUi()
{
}
