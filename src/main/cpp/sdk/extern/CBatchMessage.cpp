/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <vector>

#include "CBatchMessage.h"
#include "CCommon.h"
#include "CMessage.h"
#include "ONSFactory.h"

using std::vector;

#ifdef __cplusplus
extern "C" {
#endif

using namespace ons;

CBatchMessage* CreateBatchMessage() {
  vector<Message>* msgs = new vector<Message>();
  return (CBatchMessage*)msgs;
}

int AddMessage(CBatchMessage* batchMsg, CMessage* msg) {
  if (msg == NULL) {
    return NULL_POINTER;
  }
  if (batchMsg == NULL) {
    return NULL_POINTER;
  }
  Message* message = (Message*)msg;
  ((vector<Message>*)batchMsg)->push_back(*message);
  return OK;
}
int DestroyBatchMessage(CBatchMessage* batchMsg) {
  if (batchMsg == NULL) {
    return NULL_POINTER;
  }
  delete (vector<Message>*)batchMsg;
  return OK;
}

#ifdef __cplusplus
};
#endif
