package me.simple.lib.net;

import android.text.TextUtils;

import java.io.IOException;

import me.simple.lib.utils.LogUtils;
import okhttp3.Headers;
import okhttp3.Interceptor;
import okhttp3.MediaType;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
import okhttp3.ResponseBody;
import okio.Buffer;


public class LoggerInterceptor implements Interceptor {
    public static final String TAG = "HTTP Logger";
    private String tag;
    private boolean showResponse;

    public LoggerInterceptor(String tag, boolean showResponse)
    {
        if (TextUtils.isEmpty(tag))
        {
            tag = TAG;
        }
        this.showResponse = showResponse;
        this.tag = tag;
    }

    public LoggerInterceptor(String tag)
    {
        this(tag, false);
    }

    @Override
    public Response intercept(Chain chain) throws IOException
    {
        Request request = chain.request();

        logForRequest(request);
        long start_time = System.currentTimeMillis();
        Response response = chain.proceed(request);
        long end_time = System.currentTimeMillis();
       // LogUtils.D(TAG, "The request for " + request.url().toString() + " took " + (end_time - start_time) + " millis seconds.");
        return logForResponse(response);
    }

    private Response logForResponse(Response response)
    {
        try {
            //===>response log
          //  LogUtils.D(tag, "========response'log=======");
            Response.Builder builder = response.newBuilder();
            Response clone = builder.build();
          //  LogUtils.D(tag, "url : " + clone.request().url());
         //   LogUtils.D(tag, "code : " + clone.code());
         //   LogUtils.D(tag, "protocol : " + clone.protocol());
            if (!TextUtils.isEmpty(clone.message())){
              //  LogUtils.D(tag, "message : " + clone.message());
            }


            if (showResponse)
            {
                ResponseBody body = clone.body();
                if (body != null)
                {
                    MediaType mediaType = body.contentType();
                    if (mediaType != null)
                    {
                     //   LogUtils.D(tag, "responseBody's contentType : " + mediaType.toString());
                        if (isText(mediaType))
                        {
                            String resp = body.string();
                        //    LogUtils.D(tag, "url: " + response.request().url() + "\r\n responseBody's content : " + resp);

                            body = ResponseBody.create(mediaType, resp);
                            return response.newBuilder().body(body).build();
                        } else
                        {
                          //  LogUtils.D(tag, "responseBody's content : " + " maybe [file part] , too large too print , ignored!");
                        }
                    }
                }
            }

            LogUtils.D(tag, "========response'log=======end");
        } catch (Exception e)
        {
//            e.printStackTrace();
        }

        return response;
    }

    private void logForRequest(Request request)
    {
        try
        {
            String url = request.url().toString();
            Headers headers = request.headers();

           // LogUtils.D(tag, "========request'log=======");
          //  LogUtils.D(tag, "method : " + request.method());
         //   LogUtils.D(tag, "url : " + url);
            if (headers != null && headers.size() > 0)
            {
                LogUtils.D(tag, "headers : " + headers.toString());
            }
            RequestBody requestBody = request.body();
            if (requestBody != null)
            {
                MediaType mediaType = requestBody.contentType();
                if (mediaType != null)
                {
                   // LogUtils.D(tag, "requestBody's contentType : " + mediaType.toString());
                    if (isText(mediaType))
                    {
                      //  LogUtils.D(tag, "requestBody's content : " + bodyToString(request));
                    } else
                    {
                     //   LogUtils.D(tag, "requestBody's content : " + " maybe [file part] , too large too print , ignored!");
                    }
                }
            }
            LogUtils.D(tag, "========request'log=======end");
        } catch (Exception e)
        {
//            e.printStackTrace();
        }
    }

    private boolean isText(MediaType mediaType)
    {
        if (mediaType.type() != null && mediaType.type().equals("text"))
        {
            return true;
        }
        if (mediaType.subtype() != null)
        {
            return mediaType.subtype().equals("json") ||
                    mediaType.subtype().equals("xml") ||
                    mediaType.subtype().equals("html") ||
                    mediaType.subtype().equals("webviewhtml");
        }
        return false;
    }

    private String bodyToString(final Request request)
    {
        try
        {
            final Request copy = request.newBuilder().build();
            final Buffer buffer = new Buffer();
            copy.body().writeTo(buffer);
            return buffer.readUtf8();
        } catch (final IOException e)
        {
            return "something error when show requestBody.";
        }
    }
}
