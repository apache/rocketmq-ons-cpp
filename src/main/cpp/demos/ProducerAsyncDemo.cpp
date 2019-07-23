#include <iostream>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include "ONSFactory.h"
#include"ONSCallback.h"

using namespace std;
using namespace ons;

std::mutex m1;
std::mutex m2;
std::condition_variable cv;

class MyCallback : public SendCallbackONS {
public:

    void onSuccess(SendResultONS &sendResult) override {
        std::lock_guard<std::mutex> lg(m2);
        success_num++;
        std::cout << "send success, message_id: " << sendResult.getMessageId() << ", total: " << success_num
                  << std::endl;
        if (success_num + failed_num == total) {
            cv.notify_all();
        }
    }

    void onException(ONSClientException &e) override {
        std::lock_guard<std::mutex> lg(m2);
        failed_num++;
        std::cout << "send failure, total: " << failed_num << std::endl;
        std::cout << e.what() << std::endl;
        if (success_num + failed_num == total) {
            cv.notify_all();
        }
    }

    static int success_num;
    static int failed_num;
    static int total;
};

int MyCallback::success_num = 0;
int MyCallback::failed_num = 0;
int MyCallback::total = 32;


int main() {
    std::cout << "=======before send message=======" << std::endl;
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
    Producer *producer = nullptr;
    producer = ONSFactory::getInstance()->createProducer(factoryInfo);
    producer->start();
    Message msg(
            factoryInfo.getPublishTopics(),
            "Your Tag",
            "Your Key",
            "This message body."
    );

    auto start = std::chrono::system_clock::now();
    MyCallback m_callback;
    for (int i = 0; i < MyCallback::total; ++i) {
        producer->sendAsync(msg, &m_callback);
    }

    {
        std::unique_lock<std::mutex> lk(m1);
        cv.wait(lk);
    }

    producer->shutdown();
    std::cout << "=======after sending messages=======" << std::endl;


    return 0;
}