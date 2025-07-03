#include "jsonreader.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
DataSet JsonReader::read(const QString& filePath) {
    DataSet dataSet;
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "JSON Reader: Ошибка: не удалось открыть файл:" << file.errorString();
        return dataSet;
    }
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) {
        qWarning() << "JSON Reader: Ошибка: JSON документ должен быть массивом.";
        return dataSet;
    }
    QJsonArray jsonArray = doc.array();
    for (const QJsonValue& val : jsonArray) {
        QJsonObject obj = val.toObject();
        QJsonValue dateValue, numericValue;
        if (obj.contains("timestamp")) dateValue = obj["timestamp"];
        else if (obj.contains("date")) dateValue = obj["date"];
        else if (obj.contains("Time")) dateValue = obj["Time"];
        else continue;
        if (obj.contains("bpm")) numericValue = obj["bpm"];
        else if (obj.contains("value")) numericValue = obj["value"];
        else if (obj.contains("Value")) numericValue = obj["Value"];
        else continue;
        QString dateStr = dateValue.toString();
        QDateTime dateTime = QDateTime::fromString(dateStr, "yyyy-MM-dd hh:mm");
        if (!dateTime.isValid()) dateTime = QDateTime::fromString(dateStr, Qt::ISODate);
        if (!dateTime.isValid()) dateTime = QDateTime::fromString(dateStr, "dd.MM.yyyy hh:mm");
        if (dateTime.isValid()) {
            dataSet.append({dateTime, numericValue.toDouble()});
        } else {
            qWarning() << "JSON Reader: Не удалось распознать формат даты в строке:" << dateStr;
        }
    }
    return dataSet;
}
