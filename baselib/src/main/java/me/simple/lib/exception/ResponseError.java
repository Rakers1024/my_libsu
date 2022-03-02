package me.simple.lib.exception;


public class ResponseError extends Exception {
    private int code;
    private String description;

    public ResponseError(int code, String description) {
        super("Response code:" + code + ", description: " + description);
        this.code = code;
        this.description = description;
    }

    public ResponseError(int code) {
        super("Response code:" + code + ", description: " + ErrorCode.getErrorMsg(code));
        this.code = code;
        this.description = ErrorCode.getErrorMsg(code);
    }

    public int getCode() {
        return code;
    }

    public String getDescription() {
        return description;
    }

    public boolean isNoData() {
        return this.code == ErrorCode.NO_DATA;
    }

    @Override
    public String toString() {
        return "ResponseError{" +
                "code=" + code +
                ", description='" + description + '\'' +
                '}';
    }
}
