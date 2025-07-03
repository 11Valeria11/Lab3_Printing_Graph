#ifndef JSONREADER_H
#define JSONREADER_H
#include "interfaces.h"
class JsonReader : public IDataReader {
public:
    DataSet read(const QString& filePath) override;
};
#endif // JSONREADER_H
