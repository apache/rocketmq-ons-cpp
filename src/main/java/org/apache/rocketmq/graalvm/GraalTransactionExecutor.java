package org.apache.rocketmq.graalvm;

import com.alibaba.fastjson.JSON;
import org.apache.rocketmq.ons.api.Message;
import org.apache.rocketmq.ons.api.transaction.LocalTransactionExecuter;
import org.apache.rocketmq.ons.api.transaction.TransactionStatus;
import org.graalvm.nativeimage.CurrentIsolate;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.nativeimage.c.type.VoidPointer;

public class GraalTransactionExecutor implements LocalTransactionExecuter {
    public VoidPointer opaque;
    public CInterface.TransactionExecuteFunctionPointer transactionExecute;

    @Override
    public TransactionStatus execute(Message message, Object ob) {
        IsolateThread currentThread = CurrentIsolate.getCurrentThread();
        CTypeConversion.CCharPointerHolder pin_topic = CTypeConversion.toCString(message.getTopic());
        CTypeConversion.CCharPointerHolder pin_u_prop = CTypeConversion.toCString(JSON.toJSONString(message.getUserProperties()));
        CTypeConversion.CCharPointerHolder pin_s_prop = CTypeConversion.toCString(JSON.toJSONString(message.getSystemProperties()));
        CTypeConversion.CCharPointerHolder pin_body = CTypeConversion.toCString(new String(message.getBody()));
        try {
            int status = transactionExecute.invoke(currentThread, opaque,
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
