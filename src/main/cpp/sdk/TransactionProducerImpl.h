#ifndef __TRANSACTIONPRODUCERIMPL_H__
#define __TRANSACTIONPRODUCERIMPL_H__

#include "common/UtilAll.h"
#include "ONSFactory.h"

#include "ClientBase.h"

#include <string>

namespace ons {

    class ONSFactoryProperty;

    class TransactionProducerImpl : public TransactionProducer, public ClientBase {

    public:

        TransactionProducerImpl(ONSFactoryProperty factoryInfo, LocalTransactionChecker *pTransactionCheckListener);

        virtual ~TransactionProducerImpl();

        virtual void start();

        virtual void shutdown();

        virtual SendResultONS send(Message &msg, LocalTransactionExecuter *executor);
    };

}

#endif
