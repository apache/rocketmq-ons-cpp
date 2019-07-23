package org.apache.rocketmq.graalvm;

import com.alibaba.fastjson.JSON;
import org.apache.rocketmq.ons.api.Action;
import org.apache.rocketmq.ons.api.ConsumeContext;
import org.apache.rocketmq.ons.api.Message;
import org.apache.rocketmq.ons.api.MessageListener;
import org.graalvm.nativeimage.CurrentIsolate;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.nativeimage.c.type.VoidPointer;

public class GraalMessageListener implements MessageListener {

    public VoidPointer opaque;

    public CInterface.OnMessageFunctionPointer onMessage;


    public Action consume(Message message, ConsumeContext context) {
        IsolateThread currentThread = CurrentIsolate.getCurrentThread();
        CTypeConversion.CCharPointerHolder pin_topic = CTypeConversion.toCString(message.getTopic());
        CTypeConversion.CCharPointerHolder pin_u_prop = CTypeConversion.toCString(JSON.toJSONString(message.getUserProperties()));
        CTypeConversion.CCharPointerHolder pin_s_prop = CTypeConversion.toCString(JSON.toJSONString(message.getSystemProperties()));
        CTypeConversion.CCharPointerHolder pin_body = CTypeConversion.toCString(new String(message.getBody()));
        try {
            if (0 != onMessage.invoke(currentThread, opaque,
                pin_topic.get(),
                pin_u_prop.get(),
                pin_s_prop.get(),
                pin_body.get(),
                message.getBody().length)
            ) {
                return Action.ReconsumeLater;
            }
            return Action.CommitMessage;
        } finally {
            pin_body.close();
            pin_s_prop.close();
            pin_u_prop.close();
            pin_topic.close();
        }
    }
}
