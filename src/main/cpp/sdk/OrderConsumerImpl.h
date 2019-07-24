#ifndef MQ_ORDER_CONSUMER_IMPL_H
#define MQ_ORDER_CONSUMER_IMPL_H

#include "graal_isolate.h"

#include "OrderConsumer.h"
#include "MessageOrderListener.h"
#include "ONSFactory.h"
#include "ClientBase.h"

namespace ons {
class OrderConsumerImpl : public ons::OrderConsumer, ClientBase {
    public:
    OrderConsumerImpl(const ons::ONSFactoryProperty &property) throw (ons::ONSClientException);
    virtual ~OrderConsumerImpl();

    void start();
    void shutdown();
    void subscribe(const char* topic, const char* subExpression,
                           ons::MessageOrderListener* listener);
    };
}

#endif //MQ_ORDER_CONSUMER_IMPL_H
