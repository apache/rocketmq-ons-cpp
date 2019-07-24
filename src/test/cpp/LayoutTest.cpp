#include <type_traits>
#include <gtest/gtest.h>
#include "rocketmq.h"

TEST(LayoutTest, testIsStandardLayout) {
    ASSERT_TRUE(std::is_standard_layout<message>::value);
    ASSERT_TRUE(std::is_standard_layout<send_result>::value);
    ASSERT_TRUE(std::is_standard_layout<factory_property>::value);
    ASSERT_TRUE(std::is_standard_layout<message>::value);
}