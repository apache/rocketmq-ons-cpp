#include <iostream>
#include <chrono>
#include "ONSFactory.h"

using namespace std;
using namespace ons;

int main() {
    std::cout << "=======Before sending messages=======" << std::endl;
    ONSFactoryProperty factoryInfo;
    //Request from ONS console, this should be GroupID here.
    //请填写在阿里云ONS控制台上申请的GroupID，从实例化的版本开始，ProducerId和CounsumerId已经统一，此处设置是为了接口保持向前兼容。
    factoryInfo.setFactoryProperty(ONSFactoryProperty::ProducerId, "GID_XXX");
    //Request from ONS console
    //请填写阿里云ONS控制台上申请的topic
    factoryInfo.setFactoryProperty(ONSFactoryProperty::PublishTopics, "Your topic");
    //Your Access Key from your account.
    //请填写你的账户的AK
    factoryInfo.setFactoryProperty(ONSFactoryProperty::AccessKey, "Your Access Key");
    //Your Secret Key from your account.
    //请填写你的账户的SK
    factoryInfo.setFactoryProperty(ONSFactoryProperty::SecretKey, "Your Secret Key");
    //This is the endpoint from ONS console
    //请填写阿里云ONS控制台上对应实例的接入点
    factoryInfo.setFactoryProperty(ONSFactoryProperty::NAMESRV_ADDR,
                                   "http://xxxxxxxxxxxxxxxx.aliyuncs.com:80");
    OrderProducer *producer = nullptr;
    producer = ONSFactory::getInstance()->createOrderProducer(factoryInfo);
    producer->start();
    Message msg(
            factoryInfo.getPublishTopics(),
            "Your Tag",
            "Your Key",
            "This message body."
    );

    auto start = std::chrono::system_clock::now();
    int count = 32;
    for (int i = 0; i < count; ++i) {
        try {
            SendResultONS sendResult = producer->send(msg,"Your Sharding Key");
            std::cout << "Topic: " << msg.getTopic() << ", Message ID: " << sendResult.getMessageId() << std::endl;
        } catch (ONSClientException e) {
            std::cout << "ErrorCode: " << e.GetError() << " Exception:" << e.GetMsg() << std::endl;
        }
    }
    auto interval = std::chrono::system_clock::now() - start;
    std::cout << "Send " << count << " messages OK, costs "
              << std::chrono::duration_cast<std::chrono::milliseconds>(interval).count() << "ms" << std::endl;

    producer->shutdown();
    std::cout << "=======After sending messages=======" << std::endl;
    return 0;
}