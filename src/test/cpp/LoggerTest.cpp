#include <gtest/gtest.h>
#include "Logger.h"

//std::once_flag rocketmq::spd_log::once_log_flag;

TEST(LoggerTest, testLog) {
    rocketmq::spd_log::initLogger();
    spdlog::debug("This is a debug line");
}