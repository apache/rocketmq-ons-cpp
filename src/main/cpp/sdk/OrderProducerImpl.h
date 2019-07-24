#ifndef MQ_ORDERPRODUCERIMPL_H
#define MQ_ORDERPRODUCERIMPL_H

#include "graal_isolate.h"

#include "ONSFactory.h"
#include "OrderProducer.h"
#include "ClientBase.h"

namespace ons {
    class OrderProducerImpl : public OrderProducer, public ClientBase {
    public:
        OrderProducerImpl(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);

        virtual ~OrderProducerImpl();

        void start();

        void shutdown();

        SendResultONS send(Message &msg,std::string ShardingKey) throw(ons::ONSClientException);
    };
}  // namespace ons



#endif //MQ_ORDERPRODUCERIMPL_H
