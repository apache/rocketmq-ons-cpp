#ifndef MQ_CONSUMER_IMPL_H
#define MQ_CONSUMER_IMPL_H

#include "ONSFactory.h"
#include "PushConsumer.h"
#include <rocketmq.h>
#include "ClientBase.h"

namespace ons {

class ConsumerImpl : public PushConsumer, public ClientBase {
 public:
  ConsumerImpl();
  ConsumerImpl(const ons::ONSFactoryProperty &factoryProperty) throw(ons::ONSClientException);
  virtual ~ConsumerImpl();
  void start();
  void shutdown();
  void subscribe(const char* topic, const char* subExpression, ons::MessageListener* listener);
};

}  // namespace ons

#endif  // MQ_CONSUMER_IMPL_H
