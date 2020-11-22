#pragma once

#include <QString>
#include <QStringList>
#include <string>
#include <vector>

namespace common
{
    /**
     * @brief convert QStringList to vector of strings
     * @param list - qstringlist to convert
     * @return std::vector of strings
     */
    std::vector<std::string> toVectorOfStrings(QStringList list);
} // namespace common