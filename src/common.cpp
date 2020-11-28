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

std::vector<boost::regex> common::toVectorOfRegex(QStringList list)
{
    std::vector<boost::regex> regexes;
    for (const QString& mask : list)
    {
        regexes.emplace_back(boost::regex(mask.toStdString()));
    }
    return regexes;
}
