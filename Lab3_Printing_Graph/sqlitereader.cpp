#include "sqlitereader.h"
#include <QtSql/QtSql>
#include <QFileInfo>
#include <QDebug>
DataSet SqliteReader::read(const QString& filePath) {
    DataSet dataSet;
    const QString connectionName = "sqlite_connection_" + QFileInfo(filePath).fileName();
    QSqlDatabase db;
    if (QSqlDatabase::contains(connectionName)) {
        db = QSqlDatabase::database(connectionName);
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    }
    db.setDatabaseName(filePath);
    if (!db.open()) {
        qWarning() << "Ошибка: не удалось открыть базу данных:" << db.lastError().text();
        QSqlDatabase::removeDatabase(connectionName);
        return dataSet;
    }
    QString tableName = QFileInfo(filePath).baseName();
    QSqlQuery query(db);
    QString queryString = QString("SELECT Time, Value FROM %1").arg(tableName);
    if (!query.exec(queryString)) {
        qWarning() << "Ошибка: не удалось выполнить запрос:" << query.lastError().text();
        db.close();
        QSqlDatabase::removeDatabase(connectionName);
        return dataSet;
    }
    while (query.next()) {
        QString timeStr = query.value("Time").toString();
        double value = query.value("Value").toDouble();
        QDateTime dateTime = QDateTime::fromString(timeStr, "dd.MM.yyyy hh:mm");
        if (!dateTime.isValid()) {
            QStringList parts = timeStr.split(' ', Qt::SkipEmptyParts);
            if (parts.size() == 2) {
                QDate date = QDate::fromString(parts[0], "dd.MM.yyyy");
                bool ok;
                int minutesFromMidnight = parts[1].toInt(&ok);
                if (ok && date.isValid()) {
                    dateTime = QDateTime(date, QTime(minutesFromMidnight / 60, minutesFromMidnight % 60));
                }
            }
        }
        if (dateTime.isValid()) {
            dataSet.append({dateTime, value});
        } else {
            qWarning() << "Не удалось распознать дату-время:" << timeStr;
        }
    }
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
    return dataSet;
}
