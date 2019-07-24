#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>
#include "ONSFactory.h"


class PropertyTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }
};

TEST_F(PropertyTest, testSendMessage_AkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::Producer *producer_ = ons::ONSFactory::getInstance()->createProducer(factoryInfo);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testSendMessage_AkBeingWrong) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    ons::Producer *producer_ = ons::ONSFactory::getInstance()->createProducer(factoryInfo);
    ASSERT_TRUE(nullptr != producer_);
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            "Hello RocketMQ"
    );
    EXPECT_THROW(ons::SendResultONS
                         sendResult = producer_->send(msg), ons::ONSClientException);
    producer_->shutdown();
}

TEST_F(PropertyTest, testSendMessage_SkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::Producer *producer_ = ons::ONSFactory::getInstance()->createProducer(factoryInfo);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testSendMessage_SkBeingWrong) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GIDt_opensource_unit_test");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    ons::Producer *producer_ = ons::ONSFactory::getInstance()->createProducer(factoryInfo);
    ASSERT_TRUE(nullptr != producer_);
    ons::Message msg(
            "t_opensource_unit_test",
            "tagA",
            "ORDERID_100",
            "Hello RocketMQ"
    );
    EXPECT_THROW(ons::SendResultONS
                         sendResult = producer_->send(msg), ons::ONSClientException);
    producer_->shutdown();
}

TEST_F(PropertyTest, testSendOrderMessage_AkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_shutian_order_test");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::OrderProducer *producer_ = ons::ONSFactory::getInstance()->createOrderProducer(factoryInfo);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testSendOrderMessage_AkBeingWrong) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    ons::OrderProducer *producer_ = ons::ONSFactory::getInstance()->createOrderProducer(factoryInfo);
    ASSERT_TRUE(nullptr != producer_);
    ons::Message msg(
            "t_opensource_unit_test_order",
            "tagA",
            "ORDERID_100",
            "Hello RocketMQ"
    );
    string sharding_key = "1";
    EXPECT_THROW(ons::SendResultONS
                         sendResult = producer_->send(msg, sharding_key), ons::ONSClientException);
    producer_->shutdown();
}

TEST_F(PropertyTest, testSendOrderMessage_SkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::OrderProducer *producer_ = ons::ONSFactory::getInstance()->createOrderProducer(factoryInfo);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testSendOrderMessage_SkBeingWrong) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    ons::OrderProducer *producer_ = ons::ONSFactory::getInstance()->createOrderProducer(factoryInfo);
    ASSERT_TRUE(nullptr != producer_);
    ons::Message msg(
            "t_opensource_unit_test_order",
            "tagA",
            "ORDERID_100",
            "Hello RocketMQ"
    );
    string sharding_key = "1";
    EXPECT_THROW(ons::SendResultONS
                         sendResult = producer_->send(msg, sharding_key), ons::ONSClientException);
    producer_->shutdown();
}

TEST_F(PropertyTest, testSendTransactionMessage_AkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::TransactionProducer *producer_ = ons::ONSFactory::getInstance()->createTransactionProducer(factoryInfo,NULL);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testSendTransactionMessage_SkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ProducerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::TransactionProducer *producer_ = ons::ONSFactory::getInstance()->createTransactionProducer(factoryInfo,NULL);,
            ons::ONSClientException);
}

TEST_F(PropertyTest, testPushConsumer_AkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ConsumerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::PushConsumer *consumer_ = ons::ONSFactory::getInstance()->createPushConsumer(factoryInfo);,
            ons::ONSClientException);
}
TEST_F(PropertyTest, testPushConsumer_SkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ConsumerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::PushConsumer *consumer_ = ons::ONSFactory::getInstance()->createPushConsumer(factoryInfo);,
            ons::ONSClientException);
}
TEST_F(PropertyTest, testOrderConsumer_AkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ConsumerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::SecretKey, "SK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::OrderConsumer *consumer_ = ons::ONSFactory::getInstance()->createOrderConsumer(factoryInfo);,
            ons::ONSClientException);
}
TEST_F(PropertyTest, testOrderConsumer_SkBeingNone) {
    ons::ONSFactoryProperty factoryInfo;

    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::ConsumerId, "GID_opensource_unit_test_order");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::AccessKey, "AK");
    factoryInfo.setFactoryProperty(ons::ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://onsaddr.mq-internet-access.mq-internet.aliyuncs.com:80");
    EXPECT_THROW(
            ons::OrderConsumer *consumer_ = ons::ONSFactory::getInstance()->createOrderConsumer(factoryInfo);,
            ons::ONSClientException);
}
