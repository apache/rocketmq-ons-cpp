package org.apache.rocketmq.graalvm;

public enum ErrorCode {
    BAD_PRODUCER_INDEX(1, "Invalid producer ID"),
    SEND_MESSAGE_FAILURE(2, "Send message failure");
    private int code;
    private String desc;

    ErrorCode(int code, String desc) {
        this.code = code;
        this.desc = desc;
    }

    public int getCode() {
        return code;
    }

    public String getDesc() {
        return desc;
    }
}
