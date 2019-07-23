#ifndef __LOCALTRANSACTIONCHECKER_H__
#define __LOCALTRANSACTIONCHECKER_H__

#include "Message.h"
#include "TransactionStatus.h"

namespace ons {
class LocalTransactionChecker {
 public:
	 LocalTransactionChecker() {}
  virtual TransactionStatus check(Message& msg) = 0;
  virtual ~LocalTransactionChecker() {}
};
}

#endif
