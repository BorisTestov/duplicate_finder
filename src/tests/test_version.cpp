#include "version.h"

#include <gtest/gtest.h>

TEST(version, ok)
{
    ASSERT_EQ((int) version().find("0.0."), 0);
}
