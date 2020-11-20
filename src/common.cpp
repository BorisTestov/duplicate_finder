#include "common.h"

std::vector<std::string> common::toVectorOfStrings(QStringList list)
{
    std::vector<std::string> stringVector;
    for (const QString& value : list)
    {
        stringVector.emplace_back(value.toStdString());
    }
    return stringVector;
}
