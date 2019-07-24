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

#ifndef __PULLRESULTONS_H__
#define __PULLRESULTONS_H__

#include <vector>
#include "Message.h"
#include "ONSClient.h"

namespace ons {

    enum ONSPullStatus {
        ONS_FOUND,
        ONS_NO_NEW_MSG,
        ONS_NO_MATCHED_MSG,
        ONS_OFFSET_ILLEGAL,
        ONS_BROKER_TIMEOUT  // indicate pull request timeout or received NULL response
    };

    class ONSCLIENT_API PullResultONS {
    public:
        PullResultONS(ONSPullStatus status)
                : pullStatus(status), nextBeginOffset(0), minOffset(0), maxOffset(0) {}

        PullResultONS(ONSPullStatus pullStatus, long long nextBeginOffset,
                      long long minOffset, long long maxOffset)
                : pullStatus(pullStatus),
                  nextBeginOffset(nextBeginOffset),
                  minOffset(minOffset),
                  maxOffset(maxOffset) {}

        virtual ~PullResultONS() {}

    public:
        ONSPullStatus pullStatus;
        long long nextBeginOffset;
        long long minOffset;
        long long maxOffset;
        std::vector<Message> msgFoundList;
    };
}
#endif
