#ifndef MQ_CLIENT_BASE_H
#define MQ_CLIENT_BASE_H

#include "graal_isolate.h"

#include <mutex>
#include <unordered_map>
#include <thread>


namespace ons {

    class ClientBase {
    protected:
        ClientBase();

        virtual ~ClientBase();

        int instanceIndex_;
    };
}

#endif //MQ_CLIENT_BASE_H
