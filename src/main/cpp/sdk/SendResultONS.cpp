
#include "SendResultONS.h"
#include <string>

namespace ons {

    SendResultONS::SendResultONS() {}

    SendResultONS::~SendResultONS() {}

    void SendResultONS::setMessageId(const std::string& msgId) {
        messageId = msgId;
    }

    const char* SendResultONS::getMessageId() const {
        return messageId.c_str();
    }
}
