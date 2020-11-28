#pragma once

#include <QString>
#include <QStringList>
#include <boost/regex.hpp>
#include <string>
#include <vector>

namespace common
{
    /**
     * @brief преобразовать QStringList в вектор строк
     * @param list - qstringlist для преобразования
     * @return std::vector строк
     */
    std::vector<std::string> toVectorOfStrings(QStringList list);

    /**
     * @brief преобразовать QStringList в вектор boost::regex
     * @param list - qstringlist для преобразования
     * @return std::vector регексов
     */
    std::vector<boost::regex> toVectorOfRegex(QStringList list);
} // namespace common