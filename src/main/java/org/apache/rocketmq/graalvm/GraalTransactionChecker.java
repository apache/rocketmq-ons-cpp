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

package org.apache.rocketmq.graalvm;

import com.alibaba.fastjson.JSON;
import org.apache.rocketmq.ons.api.Message;
import org.apache.rocketmq.ons.api.transaction.LocalTransactionChecker;
import org.apache.rocketmq.ons.api.transaction.TransactionStatus;
import org.graalvm.nativeimage.CurrentIsolate;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.nativeimage.c.type.VoidPointer;


public class GraalTransactionChecker implements LocalTransactionChecker {

    public VoidPointer opaque;

    public CInterface.TransactionCheckFunctionPointer transactionCheck;

    @Override
    public TransactionStatus check(Message message) {
        IsolateThread currentThread = CurrentIsolate.getCurrentThread();
        CTypeConversion.CCharPointerHolder pin_topic = CTypeConversion.toCString(message.getTopic());
        CTypeConversion.CCharPointerHolder pin_u_prop = CTypeConversion.toCString(JSON.toJSONString(message.getUserProperties()));
        CTypeConversion.CCharPointerHolder pin_s_prop = CTypeConversion.toCString(JSON.toJSONString(message.getSystemProperties()));
        CTypeConversion.CCharPointerHolder pin_body = CTypeConversion.toCString(new String(message.getBody()));
        try {
            int status = transactionCheck.invoke(currentThread, opaque,
                pin_topic.get(),
                pin_u_prop.get(),
                pin_s_prop.get(),
                pin_body.get(),
                message.getBody().length);
            if (status == 0) {
                return TransactionStatus.CommitTransaction;
            } else if (status == 1) {
                return TransactionStatus.RollbackTransaction;
            } else {
                return TransactionStatus.Unknow;
            }
        } finally {
            pin_body.close();
            pin_s_prop.close();
            pin_u_prop.close();
            pin_topic.close();
        }
    }
}
