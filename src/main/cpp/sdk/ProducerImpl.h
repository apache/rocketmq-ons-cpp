#ifndef MQ_PRODUCER_IMPL_H
#define MQ_PRODUCER_IMPL_H

#include "graal_isolate.h"

#include "ONSFactory.h"
#include "Producer.h"
#include "ClientBase.h"

namespace ons {
    class ProducerImpl : public Producer, public ClientBase {
    public:
        ProducerImpl() throw(ons::ONSClientException);

        ProducerImpl(ONSFactoryProperty factoryProperty) throw(ons::ONSClientException);

        virtual ~ProducerImpl();

        void start();

        void shutdown();

        SendResultONS send(Message &msg) throw(ons::ONSClientException);

        SendResultONS send(Message &msg, const MessageQueueONS &mq) throw(ons::ONSClientException);

        virtual void sendAsync(Message &msg, SendCallbackONS *pSendCallback) throw(ons::ONSClientException);

        void sendOneway(Message &msg) throw(ons::ONSClientException);
    };
}  // namespace ons

#endif  // MQ_PRODUCER_IMPL_H
