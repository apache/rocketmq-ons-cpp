#ifndef MQ_ONSCLIENTABSTRACT_H
#define MQ_ONSCLIENTABSTRACT_H

#include <string>
#include <sstream>

namespace ons {
    class FAQ {
    public:
        FAQ() {}

        virtual ~FAQ() {}

        static const std::string FIND_NS_FAILED;
        static const std::string CONNECT_BROKER_FAILED;
        static const std::string SEND_MSG_TO_BROKER_TIMEOUT;
        static const std::string SERVICE_STATE_WRONG;
        static const std::string BROKER_RESPONSE_EXCEPTION;
        static const std::string CLIENT_CHECK_MSG_EXCEPTION;
        static const std::string TOPIC_ROUTE_NOT_EXIST;

        static std::string errorMessage(const std::string msgTopic,
                                        const std::string url) {
            std::stringstream ss;
            ss << msgTopic << "\nSee " << url << " for further details.";
            return ss.str();
        }
    };

}


#endif //MQ_ONSCLIENTABSTRACT_H
