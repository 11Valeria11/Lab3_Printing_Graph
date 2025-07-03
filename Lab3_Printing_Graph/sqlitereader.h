#ifndef SQLITEREADER_H
#define SQLITEREADER_H
#include "interfaces.h"
class SqliteReader : public IDataReader {
public:
    DataSet read(const QString& filePath) override;
};
#endif // SQLITEREADER_H
