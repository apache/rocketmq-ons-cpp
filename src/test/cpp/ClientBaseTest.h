#ifndef MQ_CLIENT_BASE_TEST_H
#define MQ_CLIENT_BASE_TEST_H

#include <iostream>

#include "ONSFactory.h"

#include <sys/types.h>
#include <unistd.h>

class ClientBaseTest : public testing::Test {
protected:
    void SetUp() override {
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test");
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ConsumerId, "GID_opensource_unit_test");
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::GroupId, "GID_opensource_unit_test");
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
        factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR, "47.100.33.127:9876");
    }

    void TearDown() override {
    }

    ons::ONSFactoryProperty factoryInfo;
};

#endif //MQ_CLIENT_BASE_TEST_H
