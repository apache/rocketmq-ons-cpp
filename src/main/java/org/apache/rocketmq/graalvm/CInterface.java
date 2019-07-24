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
import com.alibaba.fastjson.JSONObject;
import com.alibaba.fastjson.parser.ParserConfig;
import com.alibaba.fastjson.serializer.SerializeConfig;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.Map;
import java.util.Properties;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.atomic.AtomicInteger;
import org.apache.rocketmq.graalvm.CInterface.CPropertiesDirectives;
import org.apache.rocketmq.ons.api.Admin;
import org.apache.rocketmq.ons.api.Consumer;
import org.apache.rocketmq.ons.api.Message;
import org.apache.rocketmq.ons.api.ONSFactory;
import org.apache.rocketmq.ons.api.Producer;
import org.apache.rocketmq.ons.api.PropertyKeyConst;
import org.apache.rocketmq.ons.api.SendResult;
import org.apache.rocketmq.ons.api.exception.ONSClientException;
import org.apache.rocketmq.ons.api.order.OrderConsumer;
import org.apache.rocketmq.ons.api.order.OrderProducer;
import org.apache.rocketmq.ons.api.transaction.TransactionProducer;
import org.graalvm.nativeimage.IsolateThread;
import org.graalvm.nativeimage.c.CContext;
import org.graalvm.nativeimage.c.function.CEntryPoint;
import org.graalvm.nativeimage.c.function.CFunction;
import org.graalvm.nativeimage.c.function.CFunctionPointer;
import org.graalvm.nativeimage.c.function.InvokeCFunctionPointer;
import org.graalvm.nativeimage.c.struct.AllowNarrowingCast;
import org.graalvm.nativeimage.c.struct.AllowWideningCast;
import org.graalvm.nativeimage.c.struct.CField;
import org.graalvm.nativeimage.c.struct.CStruct;
import org.graalvm.nativeimage.c.type.CCharPointer;
import org.graalvm.nativeimage.c.type.CTypeConversion;
import org.graalvm.nativeimage.c.type.VoidPointer;
import org.graalvm.word.PointerBase;
import org.graalvm.word.UnsignedWord;
import org.graalvm.word.WordFactory;

@CContext(CPropertiesDirectives.class)
public class CInterface {

    static {
        ParserConfig.getGlobalInstance().setAsmEnable(false);
        SerializeConfig.getGlobalInstance().setAsmEnable(false);
    }

    static class CPropertiesDirectives implements CContext.Directives {

        public List<String> getOptions() {
            if ("true".equalsIgnoreCase(System.getProperty("OPEN_DUAL_ABI"))) {
                System.out.println("-D_GLIBCXX_USE_CXX11_ABI=1 -I/usr/local/include");
                return Arrays.asList("-D_GLIBCXX_USE_CXX11_ABI=1 -I/usr/local/include");
            }
            return Arrays.asList("-D_GLIBCXX_USE_CXX11_ABI=0 -I/usr/local/include");
        }

        public List<String> getHeaderFiles() {
            /*
             * The header file with the C declarations that are imported. We use a helper class that
             * locates the file in our project structure.
             */
            return Collections.singletonList("<rocketmq/rocketmq.h>");
        }
    }

    @CStruct("message") interface CMessage extends PointerBase {
        @CField("topic")
        CCharPointer getTopic();

        @CField("topic")
        void setTopic(CCharPointer value);

        @CField("tags")
        CCharPointer getTags();

        @CField("tags")
        void setTags(CCharPointer value);

        @CField("body")
        CCharPointer getBody();

        @CField("body")
        void setBody(CCharPointer value);

        @AllowWideningCast
        @CField("body_size")
        UnsignedWord getBodySize();

        @AllowNarrowingCast
        @CField("body_size")
        void setBodySize(UnsignedWord value);

        @CField("key")
        CCharPointer getKey();

        @CField("key")
        void setKey(CCharPointer value);

        @CField("user_prop")
        CCharPointer getUserProp();

        @CField("user_prop")
        void setUserProp(CCharPointer value);

        @CField("system_prop")
        CCharPointer getSystemProp();

        @CField("system_prop")
        void setSystemProp(CCharPointer value);
    }

    @CStruct("send_result") interface CSendResult extends PointerBase {
        @CField("message_id")
        CCharPointer getMessageId();

        @CField("message_id")
        void setMessageId(CCharPointer value);

        @CField("error_no")
        int getErrorCode();

        @CField("error_no")
        void setErrorCode(int value);

        @CField("error_msg")
        CCharPointer getError();

        @CField("error_msg")
        void setError(CCharPointer value);

    }

    @CStruct("factory_property") interface CFactoryProperty extends PointerBase {

        @CField("group_id")
        CCharPointer getGroupId();

        @CField("group_id")
        void setGroupId(CCharPointer value);

        @CField("access_key")
        CCharPointer getAccessKey();

        @CField("access_key")
        void setAccessKey(CCharPointer value);

        @CField("access_secret")
        CCharPointer getAccessSecret();

        @CField("access_secret")
        void setAccessSecret(CCharPointer value);

        @CField("name_srv_addr")
        CCharPointer getNameServerAddress();

        @CField("name_srv_addr")
        void setNameServerAddress(CCharPointer value);

        @CField("name_srv_domain")
        CCharPointer getNameServerDomain();

        @CField("name_srv_domain")
        void setNameServerDomain(CCharPointer value);

        @CField("message_model")
        CCharPointer getMessageModel();

        @CField("message_model")
        void setMessageModel(CCharPointer value);

        @CField("send_msg_timeout_millis")
        CCharPointer getSendMsgTimeoutMillis();

        @CField("send_msg_timeout_millis")
        void setSendMsgTimeoutMillis(CCharPointer value);

        @CField("consume_thread_nums")
        CCharPointer getConsumeThreadNums();

        @CField("consume_thread_nums")
        void setConsumeThreadNums(CCharPointer value);

        @CField("ons_channel")
        CCharPointer getOnsChannel();

        @CField("ons_channel")
        void setOnsChannel(CCharPointer value);

        @CField("max_msg_cache_size")
        CCharPointer getMaxMsgCacheSize();

        @CField("max_msg_cache_size")
        void setMaxMsgCacheSize(CCharPointer value);

        @CField("ons_trace_switch")
        CCharPointer getOnsTraceSwitch();

        @CField("ons_trace_switch")
        void setOnsTraceSwitch(CCharPointer value);

        @CField("consumer_instance_name")
        CCharPointer getConsumerInstanceName();

        @CField("consumer_instance_name")
        void setConsumerInstanceName(CCharPointer value);

        @CField("language_identifier")
        CCharPointer getLanguageIdentifier();

        @CField("language_identifier")
        void setLanguageIdentifier(CCharPointer value);

        @CField("instance_id")
        CCharPointer getInstanceId();

        @CField("instance_id")
        void setInstanceId(CCharPointer value);

        @CField("use_domain")
        int getUseDomain();

        @CField("use_domain")
        void setUseDomain(int value);
    }

    @CStruct("callback_func") interface CCallbackFunc extends PointerBase {

        @CField("send_callback_ons")
        CCharPointer getSendCallbackOns();

        @CField("send_callback_ons")
        void setSendCallbackOns(CCharPointer value);

        @CField("on_success")
        SuccessFunctionPointer getSuccessFunction();

        @CField("on_success")
        void setSuccessFunction(SuccessFunctionPointer successFunction);

        @CField("on_exception")
        ExceptionFunctionPointer getExceptionFunction();

        @CField("on_exception")
        void setExceptionFunction(ExceptionFunctionPointer exceptionFunction);
    }

    @CStruct("subscription") interface CSubscription extends PointerBase {
        @CField("topic")
        CCharPointer getTopic();

        @CField("topic")
        void setTopic(CCharPointer value);

        @CField("sub_expression")
        CCharPointer getSubExpression();

        @CField("sub_expression")
        void setSubExpression(CCharPointer value);

        @CField("on_message")
        OnMessageFunctionPointer getOnMessageFunction();

        @CField("on_message")
        void setOnMessageFunction(OnMessageFunctionPointer value);

        @CField("opaque")
        VoidPointer getOpaque();

        @CField("opaque")
        void setOpaque(VoidPointer value);
    }

    /* Import of a C function pointer type. */
    interface SuccessFunctionPointer extends CFunctionPointer {

        /*
         * Invocation of the function pointer. A call to the function is replaced with an indirect
         * call of the function pointer.
         */
        @InvokeCFunctionPointer
        void invoke(IsolateThread thread, CCharPointer cstr, CCharPointer sendCallbackONS);
    }

    /* Import of a C function pointer type. */
    interface ExceptionFunctionPointer extends CFunctionPointer {

        /*
         * Invocation of the function pointer. A call to the function is replaced with an indirect
         * call of the function pointer.
         */
        @InvokeCFunctionPointer
        void invoke(IsolateThread thread, CCharPointer cstr, int cint, CCharPointer sendCallbackONS);
    }

    interface SendCallbackFunctionPointer extends CFunctionPointer {
        @InvokeCFunctionPointer
        void invoke(IsolateThread thread, CSendResult sendResult);
    }

    interface OnMessageFunctionPointer extends CFunctionPointer {
        @InvokeCFunctionPointer
        int invoke(IsolateThread thread, VoidPointer opaque, CCharPointer topic, CCharPointer userProps,
            CCharPointer sysProps, CCharPointer body, int bodyLen);
    }

    interface TransactionCheckFunctionPointer extends CFunctionPointer {
        @InvokeCFunctionPointer
        int invoke(IsolateThread thread, VoidPointer opaque, CCharPointer topic, CCharPointer userProps,
            CCharPointer sysProps, CCharPointer body, int bodyLen);
    }

    interface TransactionExecuteFunctionPointer extends CFunctionPointer {
        @InvokeCFunctionPointer
        int invoke(IsolateThread thread, VoidPointer opaque, CCharPointer topic, CCharPointer userProps,
            CCharPointer sysProps, CCharPointer body, int bodyLen);
    }

    public static ConcurrentHashMap<Integer, Admin> instances = new ConcurrentHashMap<Integer, Admin>();

    public static AtomicInteger INDEX = new AtomicInteger();

    private static int ONS_SEND_RESULT_MSG_ID_LEN = 64;
    private static int ONS_SEND_RESULT_ERR_MSG_LEN = 1024;

    private static Properties wrapConfig(CFactoryProperty property) {
        Properties properties = new Properties();
        properties.put(PropertyKeyConst.GROUP_ID, CTypeConversion.toJavaString(property.getGroupId()));
        properties.put(PropertyKeyConst.AccessKey, CTypeConversion.toJavaString(property.getAccessKey()));
        properties.put(PropertyKeyConst.SecretKey, CTypeConversion.toJavaString(property.getAccessSecret()));
        if (property.getUseDomain() == 1) {
            //use ons address
            properties.put(PropertyKeyConst.ONSAddr, CTypeConversion.toJavaString(property.getNameServerDomain()));
        } else {
            properties.put(PropertyKeyConst.NAMESRV_ADDR, CTypeConversion.toJavaString(property.getNameServerAddress()));
        }

        String messageModel = CTypeConversion.toJavaString(property.getMessageModel());
        String consumeThreadNums = CTypeConversion.toJavaString(property.getConsumeThreadNums());
        String onsChannel = CTypeConversion.toJavaString(property.getOnsChannel());
        String maxMsgCacheSize = CTypeConversion.toJavaString(property.getMaxMsgCacheSize());
        String onsTraceSwitch = CTypeConversion.toJavaString(property.getOnsTraceSwitch());
        String consumerInstanceName = CTypeConversion.toJavaString(property.getConsumerInstanceName());
        String sendMsgTimeoutMillis = CTypeConversion.toJavaString(property.getSendMsgTimeoutMillis());
        String languageIdentifier = CTypeConversion.toJavaString(property.getLanguageIdentifier());
        String instanceId = CTypeConversion.toJavaString(property.getInstanceId());

        if (messageModel != null && !messageModel.trim().isEmpty()) {
            properties.put(PropertyKeyConst.MessageModel, messageModel);
        }
        if (consumeThreadNums != null && Integer.valueOf(consumeThreadNums) > 0) {
            properties.put(PropertyKeyConst.ConsumeThreadNums, consumeThreadNums);
        }
        if (onsChannel != null) {
            properties.put(PropertyKeyConst.OnsChannel, onsChannel);
        }
        if (maxMsgCacheSize != null) {
            properties.put(PropertyKeyConst.MaxCachedMessageSizeInMiB, maxMsgCacheSize);
        }
        if (onsTraceSwitch != null) {
            properties.put(PropertyKeyConst.MsgTraceSwitch, onsTraceSwitch);
        }
        if (consumerInstanceName != null && !consumerInstanceName.trim().isEmpty()) {
            properties.put(PropertyKeyConst.InstanceName, consumerInstanceName);
        }
        if (languageIdentifier != null) {
            properties.put(PropertyKeyConst.LANGUAGE_IDENTIFIER, languageIdentifier);
        }
        if (sendMsgTimeoutMillis != null) {
            int sendMsgTimeoutMillis_ = Integer.parseInt(sendMsgTimeoutMillis);
            if (sendMsgTimeoutMillis_ >= 100 && sendMsgTimeoutMillis_ < 3000) {
                properties.put(PropertyKeyConst.SendMsgTimeoutMillis, sendMsgTimeoutMillis);
            }
        }
        if (instanceId != null) {
            properties.put(PropertyKeyConst.INSTANCE_ID, instanceId);
        }
        return properties;
    }

    @CEntryPoint(name = "create_producer")
    public static int create_producer(IsolateThread thread, CFactoryProperty property) {
        Producer producer = ONSFactory.createProducer(wrapConfig(property));
        producer.start();
        int index = INDEX.getAndIncrement();
        instances.put(index, producer);
        return index;
    }

    @CEntryPoint(name = "create_transaction_producer")
    public static int create_transaction_producer(IsolateThread thread, CFactoryProperty property, VoidPointer checker,
        TransactionCheckFunctionPointer check) {
        GraalTransactionChecker transactionChecker = new GraalTransactionChecker();
        transactionChecker.opaque = checker;
        transactionChecker.transactionCheck = check;
        TransactionProducer producer = ONSFactory.createTransactionProducer(wrapConfig(property), transactionChecker);
        producer.start();
        int index = INDEX.getAndIncrement();
        instances.put(index, producer);
        return index;
    }

    @CEntryPoint(name = "create_consumer")
    public static int create_consumer(IsolateThread thread, CFactoryProperty property) {
        Consumer consumer = ONSFactory.createConsumer(wrapConfig(property));
        int index = INDEX.getAndIncrement();
        instances.put(index, consumer);
        return index;
    }

    @CEntryPoint(name = "create_order_consumer")
    public static int create_order_consumer(IsolateThread thread, CFactoryProperty property) {
        OrderConsumer consumer = ONSFactory.createOrderedConsumer(wrapConfig(property));
        int index = INDEX.getAndIncrement();
        instances.put(index, consumer);
        return index;
    }

    @CEntryPoint(name = "subscribe")
    public static void subscribe(IsolateThread thread, int instanceIndex, CSubscription cSub) {
        Admin instance = instances.get(instanceIndex);
        if (instance instanceof Consumer) {
            Consumer consumer = (Consumer) instance;
            GraalMessageListener messageListener = new GraalMessageListener();
            messageListener.opaque = cSub.getOpaque();
            messageListener.onMessage = cSub.getOnMessageFunction();
            consumer.subscribe(CTypeConversion.toJavaString(cSub.getTopic()),
                CTypeConversion.toJavaString(cSub.getSubExpression()),
                messageListener);
            //System.out.println("Subscribe OK");
            return;
        }
        //System.out.println("Subscribe failed");
    }

    @CEntryPoint(name = "subscribe_order_listener")
    public static void subscribe_order_message_listener(IsolateThread thread, int instanceIndex, CSubscription cSub) {
        Admin instance = instances.get(instanceIndex);
        if (instance instanceof OrderConsumer) {
            OrderConsumer consumer = (OrderConsumer) instance;
            GraalMessageOrderListener messageListener = new GraalMessageOrderListener();
            messageListener.opaque = cSub.getOpaque();
            messageListener.onMessage = cSub.getOnMessageFunction();
            consumer.subscribe(CTypeConversion.toJavaString(cSub.getTopic()),
                CTypeConversion.toJavaString(cSub.getSubExpression()),
                messageListener);
            //System.out.println("Subscribe OK");
            return;
        }
        //System.out.println("Subscribe failed");
    }

    @CEntryPoint(name = "start_instance")
    public static void start_instance(IsolateThread thread, int instanceIndex) {
        Admin instance = instances.get(instanceIndex);
        if (null != instance) {
            instance.start();
        }
    }

    @CEntryPoint(name = "destroy_instance")
    public static void destroy_instance(IsolateThread thread, int index) {
        instances.get(index).shutdown();
        instances.remove(index);
    }

    @CEntryPoint(name = "create_order_producer")
    public static int create_order_producer(IsolateThread thread, CFactoryProperty property) {
        OrderProducer orderProducer = ONSFactory.createOrderProducer(wrapConfig(property));
        orderProducer.start();
        int index = INDEX.getAndIncrement();
        instances.put(index, orderProducer);
        return index;
    }

    @CEntryPoint(name = "send_message")
    public static void send_message(IsolateThread thread, int producerId, CMessage cMsg, CSendResult sendResult) {
        Admin instance = instances.get(producerId);
        CTypeConversion.CCharPointerHolder pin = null;
        try {
            if (instance instanceof Producer) {
                try {
                    Message msg = message_transformer(cMsg);
                    SendResult result = ((Producer) instance).send(msg);
                    sendResult.setErrorCode(0);
                    //sendResult.setError(CTypeConversion.toCString(null).get());
                    //sendResult.setMessageId(CTypeConversion.toCString(result.getMessageId()).get());
                    pin = CTypeConversion.toCString(result.getMessageId());
                    int len = Math.min(ONS_SEND_RESULT_MSG_ID_LEN, result.getMessageId().length());
                    memmove(sendResult.getMessageId(), pin.get(), WordFactory.unsigned(len));
                } catch (ONSClientException e) {
                    sendResult.setErrorCode(ErrorCode.SEND_MESSAGE_FAILURE.getCode());
                    pin = CTypeConversion.toCString(e.getMessage());
                    memmove(sendResult.getError(), pin.get(),
                        WordFactory.unsigned(Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, e.getMessage().length())));
                }
            } else {
                sendResult.setErrorCode(ErrorCode.BAD_PRODUCER_INDEX.getCode());
                pin = CTypeConversion.toCString(ErrorCode.BAD_PRODUCER_INDEX.getDesc());
                int length = Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, ErrorCode.BAD_PRODUCER_INDEX.getDesc().length());
                memmove(sendResult.getError(), pin.get(), WordFactory.unsigned(length));
            }
        } finally {
            if (null != pin) {
                pin.close();
            }
        }
    }

    @CEntryPoint(name = "send_message_oneway")
    public static void send_message_oneway(IsolateThread thread, int producerId, CMessage cMsg,
        CSendResult sendResult) {
        Admin instance = instances.get(producerId);
        CTypeConversion.CCharPointerHolder pin = null;
        try {
            if (instance instanceof Producer) {
                Message message = message_transformer(cMsg);
                try {
                    ((Producer) instance).sendOneway(message);
                } catch (ONSClientException e) {
                    sendResult.setErrorCode(ErrorCode.SEND_MESSAGE_FAILURE.getCode());
                    pin = CTypeConversion.toCString(e.getMessage());
                    int len = Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, e.getMessage().length());
                    memmove(sendResult.getError(), pin.get(), WordFactory.unsigned(len));
                }
            }
        } finally {
            if (null != pin) {
                pin.close();
            }
        }
    }

    @CFunction
    protected static native PointerBase memmove(PointerBase dest, PointerBase src, UnsignedWord n);

    @CEntryPoint(name = "send_message_async")
    public static void send_message_async(final IsolateThread thread, int producerId, final CMessage cMessage,
        final CSendResult cSendResult, final CCallbackFunc cCallbackFunc) {

        Admin instance = instances.get(producerId);
        if (instance instanceof Producer) {
            Message message = message_transformer(cMessage);
            GraalSendCallback mcb = new GraalSendCallback();
            mcb.sendCallbackONS = cCallbackFunc.getSendCallbackOns();
            mcb.exceptionFunctionPtr = cCallbackFunc.getExceptionFunction();
            mcb.successFunctionPtr = cCallbackFunc.getSuccessFunction();
            mcb.message = cMessage.getBody();
            CTypeConversion.CCharPointerHolder pin = null;
            try {
                ((Producer) instance).sendAsync(message, mcb);
            } catch (ONSClientException e) {
                cSendResult.setErrorCode(ErrorCode.SEND_MESSAGE_FAILURE.getCode());
                pin = CTypeConversion.toCString(e.getMessage());
                memmove(cSendResult.getError(), pin.get(),
                    WordFactory.unsigned(Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, e.getMessage().length())));
            } finally {
                if (null != pin) {
                    pin.close();
                }
            }
        }
    }

    @CEntryPoint(name = "send_message_transaction")
    public static void send_message_transaction(final IsolateThread thread, int producerId, final CMessage cMessage,
        final CSendResult sendResult, VoidPointer executor, TransactionExecuteFunctionPointer execute) {
        Admin instance = instances.get(producerId);
        CTypeConversion.CCharPointerHolder pin = null;
        try {
            if (instance instanceof TransactionProducer) {
                try {
                    Message message = message_transformer(cMessage);
                    GraalTransactionExecutor transactionExecutor = new GraalTransactionExecutor();
                    transactionExecutor.opaque = executor;
                    transactionExecutor.transactionExecute = execute;
                    SendResult result = ((TransactionProducer) instance).send(message, transactionExecutor, null);
                    sendResult.setErrorCode(0);
                    pin = CTypeConversion.toCString(result.getMessageId());
                    int len = Math.min(ONS_SEND_RESULT_MSG_ID_LEN, result.getMessageId().length());
                    memmove(sendResult.getMessageId(), pin.get(), WordFactory.unsigned(len));
                } catch (Exception e) {
                    sendResult.setErrorCode(ErrorCode.SEND_MESSAGE_FAILURE.getCode());
                    pin = CTypeConversion.toCString(e.getMessage());
                    memmove(sendResult.getError(), pin.get(),
                        WordFactory.unsigned(Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, e.getMessage().length())));
                }
            } else {
                sendResult.setErrorCode(ErrorCode.BAD_PRODUCER_INDEX.getCode());
                sendResult.setError(CTypeConversion.toCString(ErrorCode.BAD_PRODUCER_INDEX.getDesc()).get());
                pin = CTypeConversion.toCString(ErrorCode.BAD_PRODUCER_INDEX.getDesc());
                int len = Math.min(ONS_SEND_RESULT_MSG_ID_LEN, ErrorCode.BAD_PRODUCER_INDEX.getDesc().length());
                memmove(sendResult.getError(), pin.get(), WordFactory.unsigned(len));
            }
        } finally {
            if (null != pin) {
                pin.close();
            }
        }
    }

    @CEntryPoint(name = "send_order_message")
    public static void send_order_message(IsolateThread thread, int producerId, CMessage cMsg, CSendResult sendResult,
        CCharPointer shardingKey) {
        Admin instance = instances.get(producerId);
        CTypeConversion.CCharPointerHolder pin = null;
        try {
            if (instance instanceof OrderProducer) {
                Message msg = message_transformer(cMsg);
                try {
                    SendResult result = ((OrderProducer) instance).send(msg, CTypeConversion.toJavaString(shardingKey));
                    sendResult.setErrorCode(0);
                    pin = CTypeConversion.toCString(result.getMessageId());
                    memmove(sendResult.getMessageId(), pin.get(),
                        WordFactory.unsigned(Math.min(ONS_SEND_RESULT_MSG_ID_LEN, result.getMessageId().length())));
                } catch (ONSClientException e) {
                    sendResult.setErrorCode(ErrorCode.SEND_MESSAGE_FAILURE.getCode());
                    pin = CTypeConversion.toCString(e.getMessage());
                    memmove(sendResult.getError(), pin.get(),
                        WordFactory.unsigned(Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, e.getMessage().length())));
                }
            } else {
                sendResult.setErrorCode(ErrorCode.BAD_PRODUCER_INDEX.getCode());
                int length = Math.min(ONS_SEND_RESULT_ERR_MSG_LEN, ErrorCode.BAD_PRODUCER_INDEX.getDesc().length());
                pin = CTypeConversion.toCString(ErrorCode.BAD_PRODUCER_INDEX.getDesc());
                memmove(sendResult.getError(), pin.get(), WordFactory.unsigned(length));
            }
        } finally {
            if (null != pin) {
                pin.close();
            }
        }
    }

    private static Message message_transformer(CMessage cMsg) {
        Message msg = new Message(
            CTypeConversion.toJavaString(cMsg.getTopic()),
            CTypeConversion.toJavaString(cMsg.getTags()),
            CTypeConversion.toJavaString(cMsg.getBody(), cMsg.getBodySize()).getBytes());
        String userProps = CTypeConversion.toJavaString(cMsg.getUserProp());
        assignProperties(userProps, msg, false);

        String sysProps = CTypeConversion.toJavaString(cMsg.getSystemProp());
        assignProperties(sysProps, msg, true);
        return msg;
    }

    private static void assignProperties(String json, Message msg, boolean sys) {
        JSONObject root = JSON.parseObject(json);
        for (Map.Entry<String, Object> entry : root.entrySet()) {
            if (sys) {
                msg.putSystemProperties(entry.getKey(), String.valueOf(entry.getValue()));
            } else {
                msg.putUserProperties(entry.getKey(), String.valueOf(entry.getValue()));
            }
        }
    }

    public static void main(String[] args) {
    }

}
