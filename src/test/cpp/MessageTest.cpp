#include <iostream>
#include <chrono>
#include <gtest/gtest.h>
#include <memory>

#include "Message.h"
#include "ONSFactory.h"

class MessageTest : public testing::Test {
protected:
    void SetUp() override {
        ubody_ = new unsigned char[10];
        memset(ubody_, 0, 10);
        strcpy(reinterpret_cast<char *>(ubody_), "RocketMQ");
        sbody = std::string("RocketMQ");
    }

    void TearDown() override {
        delete ubody_;
    }

    const char *topic_ = "Topic";
    const char *tag_ = "Tag";
    const char *key_ = "Key";
    const char *body_ = "RocketMQ";
    const char *user_key_ = "UserKey";
    const char *user_value_ = "UserValue";
    const char *sys_key_ = "SysKey";
    const char *sys_value_ = "SysValue";
    unsigned char *ubody_;
    int bodylen = strlen("RocketMQ");
    std::string sbody;
};

TEST_F(MessageTest, testMessage_TopicBeingNULL) {
    EXPECT_THROW(
            ons::Message msg(NULL, tag_, key_, body_);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageWithoutKey_TopicBeingNULL) {
    EXPECT_THROW(
            ons::Message msg(NULL, tag_, body_);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessage_TopicLenBeingZero) {
    EXPECT_THROW(
            ons::Message msg(topic_, 0, tag_, strlen(tag_), body_, strlen(body_));,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessage_BodyBeingNULL) {
    EXPECT_THROW(
            ons::Message msg(topic_, tag_, key_, NULL);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageWithoutKey_BodyBeingNULL) {
    EXPECT_THROW(
            ons::Message msg(topic_, tag_, NULL);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessage_BodyLenBeingZero) {
    EXPECT_THROW(
            ons::Message msg(topic_, strlen(topic_), tag_, strlen(tag_), body_, 0);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageSysPerperty_KeyBeingNULL) {
    ons::Message msg(topic_, tag_, key_, body_);
    EXPECT_THROW(
            msg.putSystemProperties(NULL, sys_value_);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageSysPerperty_ValueBeingNULL) {
    ons::Message msg(topic_, tag_, key_, body_);
    EXPECT_THROW(
            msg.putSystemProperties(sys_key_, NULL);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageUserPerperty_KeyBeingNULL) {
    ons::Message msg(topic_, tag_, key_, body_);
    EXPECT_THROW(
            msg.putSystemProperties(NULL, user_value_);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageUserPerperty_ValueBeingNULL) {
    ons::Message msg(topic_, tag_, key_, body_);
    EXPECT_THROW(
            msg.putSystemProperties(user_key_, NULL);,
            ons::ONSClientException);
}

TEST_F(MessageTest, testMessageGetTopic_ValueBeingNormal) {
    ons::Message msg(topic_, tag_, key_, body_);
    ASSERT_STREQ(topic_, msg.getTopic());
}

TEST_F(MessageTest, testMessageetTopic_ValueBeingNormal) {
    ons::Message msg("", tag_, key_, body_);
    msg.setTopic(topic_);
    ASSERT_STREQ(topic_, msg.getTopic());
}

TEST_F(MessageTest, testMessageGetTopic_ValueBeingNull) {
    ons::Message msg("", tag_, key_, body_);
    ASSERT_STREQ("", msg.getTopic());
}

TEST_F(MessageTest, testMessageSetTopic_ValueBeingNull) {
    ons::Message msg("", tag_, key_, body_);
    msg.setTopic(NULL);
    ASSERT_STREQ("", msg.getTopic());
}

TEST_F(MessageTest, testMessageGetTag_ValueBeingNormal) {
    ons::Message msg(topic_, tag_, key_, body_);
    ASSERT_STREQ(tag_, msg.getTag());
}

TEST_F(MessageTest, testMessageSetTag_ValueBeingNormal) {
    ons::Message msg(topic_, "", key_, body_);
    msg.setTag(tag_);
    ASSERT_STREQ(tag_, msg.getTag());
}

TEST_F(MessageTest, testMessageGetTag_ValueBeingNull) {
    ons::Message msg(topic_, "", key_, body_);
    ASSERT_STREQ("", msg.getTag());
}

TEST_F(MessageTest, testMessageSetTag_ValueBeingNull) {
    ons::Message msg(topic_, "", key_, body_);
    msg.setTag(NULL);
    ASSERT_STREQ("", msg.getTag());
}

TEST_F(MessageTest, testMessageGetBody_ValueBeingNormal) {
    ons::Message msg(topic_, tag_, key_, body_);
    ASSERT_STREQ(body_, msg.getBody());
}

TEST_F(MessageTest, testMessageSetBody_ValueBeingNormal) {
    ons::Message msg(topic_, tag_, key_, "");
    msg.setBody(ubody_, bodylen);
    ASSERT_STREQ(body_, msg.getBody());
}

TEST_F(MessageTest, testMessageGetByteBody_ValueBeingNormal) {
    int len = 0;
    ons::Message msg(topic_, tag_, key_, body_);
    const char *strBody = msg.getByteBody(&len);
    ASSERT_STREQ(body_, strBody);
    ASSERT_EQ(strlen(body_), len);
}

TEST_F(MessageTest, testMessageSetBodyWithLen_ValueBeingNormal) {
    int len = 0;
    ons::Message msg(topic_, tag_, key_, "");
    msg.setBody(ubody_, bodylen);
    const char *strBody = msg.getByteBody(&len);
    ASSERT_STREQ(body_, strBody);
    ASSERT_EQ(bodylen, len);
}

TEST_F(MessageTest, testMessageGetBody_ValueBeingNull) {
    ons::Message msg(topic_, tag_, key_, "");
    ASSERT_STREQ("", msg.getBody());
}

TEST_F(MessageTest, testMessageSetBody_ValueBeingNull) {
    ons::Message msg(topic_, tag_, key_, "");
    msg.setBody(NULL, strlen(body_));
    ASSERT_STREQ("", msg.getBody());
}

TEST_F(MessageTest, testMessageGetMsgBody_ValueBeingString) {
    ons::Message msg(topic_, tag_, key_, body_);
    ASSERT_STREQ(sbody.c_str(), msg.getMsgBody().c_str());
}

TEST_F(MessageTest, testMessageGetMsgBodyLen_ValueBeingString) {
    ons::Message msg(topic_, tag_, key_, body_);
    ASSERT_EQ(sbody.length(), msg.getBodySize());
}

TEST_F(MessageTest, testMessageSetMsgBody_ValueBeingString) {
    ons::Message msg(topic_, tag_, key_, "");
    msg.setMsgBody(sbody);
    ASSERT_STREQ(sbody.c_str(), msg.getMsgBody().c_str());
}

TEST_F(MessageTest, testMessageGetUserPerperty_KeyBeingNull) {
    ons::Message msg(topic_, tag_, key_, "");
    ASSERT_TRUE(NULL == msg.getUserProperties(NULL));
}

TEST_F(MessageTest, testMessageGetUserPerperty_KeyNotExsit) {
    ons::Message msg(topic_, tag_, key_, "");
    ASSERT_STREQ("", msg.getUserProperties("NONEEXISTKEY"));
}

TEST_F(MessageTest, testMessageGetSysPerperty_KeyBeingNull) {
    ons::Message msg(topic_, tag_, key_, "");
    ASSERT_TRUE(NULL == msg.getSystemProperties(NULL));
}

TEST_F(MessageTest, testMessageGetSysPerperty_KeyNotExsit) {
    ons::Message msg(topic_, tag_, key_, "");
    ASSERT_STREQ("", msg.getUserProperties("NONEEXISTKEY"));
}

TEST_F(MessageTest, testMessageSetDeliverTime_ValueBeingNormal) {
    ons::Message msg(topic_, tag_, key_, body_);
    long long now = 123456789L;
    msg.setStartDeliverTime(now);
    ASSERT_EQ(now, msg.getStartDeliverTime());
}

TEST_F(MessageTest, testMessageSetDeliverTime_ValueBeingZero) {
    ons::Message msg(topic_, tag_, key_, body_);
    long long now = 0;
    msg.setStartDeliverTime(now);
    ASSERT_EQ(now, msg.getStartDeliverTime());
}

TEST_F(MessageTest, testMessageSetDeliverTime_ValueBeingNagetive) {
    ons::Message msg(topic_, tag_, key_, body_);
    long long now = -10000;
    msg.setStartDeliverTime(now);
    ASSERT_EQ(now, msg.getStartDeliverTime());
}
