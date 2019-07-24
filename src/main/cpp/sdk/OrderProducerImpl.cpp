#include "OrderProducerImpl.h"
#include "rocketmq.h"
#include "rocketmq-ons-cpp-full.h"
#include <iostream>
#include "SendResultONS.h"
#include "Logger.h"
#include "common/UtilAll.h"

using namespace ons;

OrderProducerImpl::OrderProducerImpl(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException) {
    std::string logPath(factoryProperty.getLogPath());
    rocketmq::spd_log::initLogger(logPath, rocketmq::LOGLEVEL_INFO);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    factory_property fp;
    FactoryPropertyConverter converter(factoryProperty, fp);
    instanceIndex_ = create_order_producer(thread_, &fp);
    spdlog::info("Create Order Producer OK, InstanceId:{}, ProducerID:{}, NameServer:{}",
                 instanceIndex_, factoryProperty.getProducerId(), factoryProperty.getNameSrvAddr());
}

OrderProducerImpl::~OrderProducerImpl() {
    rocketmq::spd_log::destoryLogger();
}

void OrderProducerImpl::start() {

}

void OrderProducerImpl::shutdown() {
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    destroy_instance(thread_, instanceIndex_);
    spdlog::info("Destroy Order Producer instance {} OK", instanceIndex_);
}

SendResultONS OrderProducerImpl::send(Message &msg, std::string shardingKey) throw(ons::ONSClientException) {
    message m;
    MessageConverter converter(m, msg);
    send_result sendResult;
    SendResultWrapper wrapper(sendResult);
    graal_isolatethread_t *thread_;
    ThreadAttachment attachment(&thread_);
    send_order_message(thread_, instanceIndex_, &m, &sendResult,(char*)shardingKey.c_str());
    if (sendResult.error_no) {
        ONSClientException clientException(std::string(sendResult.error_msg), sendResult.error_no);
        throw clientException;
    }
    spdlog::debug("Send message OK. MsgId: {}", sendResult.message_id);
    SendResultONS sendResultOns;
    sendResultOns.setMessageId(std::string(sendResult.message_id));
    return sendResultOns;

}