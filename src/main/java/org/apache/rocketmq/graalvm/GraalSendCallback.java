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

import org.apache.rocketmq.ons.api.OnExceptionContext;
import org.apache.rocketmq.ons.api.SendCallback;
import org.apache.rocketmq.ons.api.SendResult;
import org.graalvm.nativeimage.CurrentIsolate;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.word.WordFactory;

public class GraalSendCallback implements SendCallback, Cloneable {
    public CInterface.SuccessFunctionPointer successFunctionPtr;
    public CInterface.ExceptionFunctionPointer exceptionFunctionPtr;
    public CCharPointer sendCallbackONS;
    public CCharPointer message;

    public void onSuccess(SendResult var1) {
        IsolateThread currentThread = CurrentIsolate.getCurrentThread();
        CTypeConversion.CCharPointerHolder pin = CTypeConversion.toCString(var1.getMessageId());
        successFunctionPtr.invoke(currentThread, pin.get(), sendCallbackONS);
        pin.close();
    }

    public void onException(OnExceptionContext var1) {
        IsolateThread currentThread = CurrentIsolate.getCurrentThread();
        exceptionFunctionPtr.invoke(currentThread, message, ErrorCode.SEND_MESSAGE_FAILURE.getCode(), sendCallbackONS);
    }
}
