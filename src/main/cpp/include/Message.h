#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <map>
#include <sstream>
#include <vector>
#include "ONSClient.h"

namespace ons {

class  SystemPropKey {
 public:
  SystemPropKey() {}
  ~SystemPropKey() {}
  static const char* TAG;
  static const char* KEY;
  static const char* MSGID;
  static const char* RECONSUMETIMES;
  static const char* STARTDELIVERTIME;
};

class  ONSCLIENT_API Message {
 public:
  Message();
  Message(const std::string& topic, const std::string& tags, const std::string& byte_body);
  Message(const char* topic, const char* tags, const char* byte_body);
  Message(const char* topic, size_t topic_size, const char* tags, size_t tags_size, const char* body, size_t body_size);
  Message(const char* topic, const char* tags, const char* keys, const char* body);

  virtual ~Message();

  Message(const Message& other);
  Message& operator=(const Message& other);

  // userProperties was used to save user specific parameters which doesn't
  // belong to SystemPropKey
  void putUserProperties(const char* key, const char* value);
	const char* getUserProperties(const char* key) const;
  void setUserProperties(std::map<std::string, std::string>& userProperty);
  std::map<std::string, std::string> getUserProperties() const;

  // systemProperties only save parameters defined in SystemPropKey, please do
  // not add other parameters into systemProperties, else it was not saved.
  void putSystemProperties(const char* key, const char* value);
	const char* getSystemProperties(const char* key) const;
  void setSystemProperties(std::map<std::string, std::string>& systemProperty);
  std::map<std::string, std::string> getSystemProperties() const;

	const char* getTopic() const;
  void setTopic(const char* topic);

	const char* getTag() const;
  void setTag(const char* tags);

	const char* getKey() const;
  void setKey(const char* keys);

	const char* getMsgID() const;
  void setMsgID(const char* msgId);

  const long long getStartDeliverTime() const;
  void setStartDeliverTime(long long level);

  const char* getBody() const;
  const char* getByteBody(int *len) const;
  const std::string getMsgBody() const;
  const size_t getBodySize() const;
  void setMsgBody(const std::string msgbody);
  void setBody(unsigned char* byte_msgbody, int len);

  const int getReconsumeTimes() const;
  void setReconsumeTimes(int reconsumeTimes);

  long long getStoreTimestamp() const;
  void setStoreTimestamp(long long storeTimestamp);

  const std::string toString() const;

  const std::string toSystemString() const;

  const std::string toUserString() const;

  long long getQueueOffset() const;
  void setQueueOffset(long long queueOffset);
 protected:
  void Init(const std::string& topic, const std::string& tags,
            const std::string& keys, const std::string& body);

 private:
  std::string topic;
  std::string body;
  size_t body_size;
  long long m_storeTimestamp;
  long long m_queueOffset;
  std::map<std::string, std::string> systemProperties;
  std::map<std::string, std::string> userProperties;
};

}  //<!end namespace;
#endif
