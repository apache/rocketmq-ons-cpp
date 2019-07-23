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
