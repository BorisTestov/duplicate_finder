#include "common.h"

#include <gtest/gtest.h>

TEST(vectorOfString, ok)
{
    // @todo add fixture and parameters
    QStringList data;
    std::string v1 = "value 1";
    std::string v2 = "value 2";
    std::string v3 = "value 3";
    data << QString::fromStdString(v1)
         << QString::fromStdString(v2)
         << QString::fromStdString(v3);
    std::vector<std::string> stringData = common::toVectorOfStrings(data);
    ASSERT_EQ(stringData.at(0), v1);
    ASSERT_EQ(stringData.at(1), v2);
    ASSERT_EQ(stringData.at(2), v3);
}

TEST(vectorOfRegex, ok)
{
    QStringList data;
    std::string v1 = "value 1";
    std::string v2 = "value 2";
    std::string v3 = "value 3";
    data << QString::fromStdString(v1)
         << QString::fromStdString(v2)
         << QString::fromStdString(v3);
    std::vector<boost::regex> regexData = common::toVectorOfRegex(data);
    ASSERT_EQ(regexData.at(0), boost::regex(v1));
    ASSERT_EQ(regexData.at(1), boost::regex(v2));
    ASSERT_EQ(regexData.at(2), boost::regex(v3));
}
