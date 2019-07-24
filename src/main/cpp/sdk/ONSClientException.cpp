#include "ONSClientException.h"
#include <string>

namespace ons {

    ONSClientException::ONSClientException() throw() {}
    ONSClientException::~ONSClientException() throw() {}
    ONSClientException::ONSClientException(std::string msg, int error) throw()
            : m_msg(msg), m_error(error) {}

    const char* ONSClientException::what() const throw() { return m_msg.c_str(); }
    const char* ONSClientException::GetMsg() const throw() { return m_msg.c_str(); }
    int ONSClientException::GetError() const throw() { return m_error; }
}
