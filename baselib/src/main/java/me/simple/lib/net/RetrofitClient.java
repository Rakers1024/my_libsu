package me.simple.lib.net;

import androidx.lifecycle.LifecycleOwner;

import com.uber.autodispose.AutoDispose;
import com.uber.autodispose.AutoDisposeConverter;
import com.uber.autodispose.android.lifecycle.AndroidLifecycleScopeProvider;

import java.io.ByteArrayOutputStream;
import java.io.InputStream;
import java.net.Proxy;
import java.util.Map;
import java.util.concurrent.TimeUnit;

import io.reactivex.ObservableTransformer;
import io.reactivex.Observer;
import io.reactivex.SingleObserver;
import io.reactivex.SingleTransformer;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.annotations.NonNull;
import io.reactivex.functions.Function;
import io.reactivex.schedulers.Schedulers;
import me.simple.lib.exception.ResponseError;
import me.simple.lib.utils.HttpsUtils;
import me.simple.lib.utils.LogUtils;
import me.simple.lib.utils.SafeUtils;
import okhttp3.Interceptor;
import okhttp3.OkHttpClient;
import okhttp3.ResponseBody;
import retrofit2.Response;
import retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;
import retrofit2.converter.gson.GsonConverterFactory;

public class RetrofitClient {
    private static final String TAG = "RetrofitClient";

    private static final long DEFAULT_TIMEOUT = 3;

    private static volatile RetrofitClient INSTANCE;
    private ApiService apiService;
    private boolean isInited = false;

    private RetrofitClient() {

    }

    public void init(String url, String rc4key, String appid) {
        if (isInited) {
            return;
        }
        isInited = true;
        SafeUtils.rc4key = rc4key;
        SafeUtils.appid = appid;
        HttpsUtils.SSLParams sslParams = HttpsUtils.getSslSocketFactory(null, null, null);
        OkHttpClient okHttpClient = createOkHttpClient(sslParams);
        retrofit2.Retrofit retrofit = new retrofit2.Retrofit.Builder()
                .client(okHttpClient)
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .baseUrl(url)
                .build();
        apiService = retrofit.create(ApiService.class);
    }

    public void OOoooOooOOooOO(String url,  String rc4key,String appid,String key,String key2,String key3,String key4) {
        if (isInited) {
            return;
        }
        isInited = true;
        SafeUtils.rc4key = rc4key;
        SafeUtils.appid = appid;
        HttpsUtils.SSLParams sslParams = HttpsUtils.getSslSocketFactory(null, null, null);
        OkHttpClient okHttpClient = createOkHttpClient(sslParams);
        retrofit2.Retrofit retrofit = new retrofit2.Retrofit.Builder()
                .client(okHttpClient)
                .addConverterFactory(GsonConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .baseUrl(url)
                .build();
        apiService = retrofit.create(ApiService.class);
    }

    private OkHttpClient createOkHttpClient(HttpsUtils.SSLParams sslParams) {
        return new OkHttpClient.Builder()
                .proxy(Proxy.NO_PROXY)
                .connectTimeout(DEFAULT_TIMEOUT, TimeUnit.SECONDS)
                .writeTimeout(DEFAULT_TIMEOUT, TimeUnit.SECONDS)
                .readTimeout(DEFAULT_TIMEOUT, TimeUnit.SECONDS)
                .retryOnConnectionFailure(true)
                .hostnameVerifier((hostname, session) -> true)
                .sslSocketFactory(sslParams.sSLSocketFactory, sslParams.trustManager)
                .addInterceptor(new LoggerInterceptor(null, true))
                .build();
    }

    public static synchronized RetrofitClient getInstance() {
        if (INSTANCE == null) {
            synchronized (RetrofitClient.class) {
                if (INSTANCE == null) {
                    INSTANCE = new RetrofitClient();
                }
            }
        }
        return INSTANCE;
    }


    /**
     * Get请求，直接请求，返回clazz类型的数据, Lazy加载
     *
     * @param url
     * @param parameters
     * @param subscriber
     */
    @SuppressWarnings("unchecked")
    public void get(String url, Map<String, String> parameters, LifecycleOwner lifecycleOwner, SingleObserver subscriber) {
        apiService.executeGet(url, parameters)
                .compose(switchSchedulers())
                .compose(dealWithError())
                .map(new GsonFunctionMap<>(parameters.get("t"), subscriber.getClass()))
                .as(bindLifecycle(lifecycleOwner))
                .subscribe(subscriber);
    }

    @SuppressWarnings("unchecked")
    public void get(Map<String, String> parameters, SingleObserver subscriber) {
        apiService.executeGet("", parameters)
                .compose(switchSchedulers())
                .compose(dealWithError())
                .map(new GsonFunctionMap<>(parameters.get("t"), subscriber.getClass()))
                .subscribe(subscriber);
    }


    public void downLoad(String url, Observer<byte[]> observable) {
        apiService.download(url)
                .subscribeOn(Schedulers.io())
                .observeOn(Schedulers.io())
                .map(new Function<ResponseBody, byte[]>() {
                    @Override
                    public byte[] apply(@NonNull ResponseBody responseBody) throws Exception {
                        InputStream inputStream = responseBody.byteStream();
                        ByteArrayOutputStream output = new ByteArrayOutputStream();
                        byte[] buffer = new byte[1024 * 4];
                        int n = 0;
                        while (-1 != (n = inputStream.read(buffer))) {
                            output.write(buffer, 0, n);
                        }
                        return output.toByteArray();
                    }
                }).observeOn(AndroidSchedulers.mainThread())
                .subscribe(observable);
    }

    /**
     * Get请求，直接请求，返回clazz类型的数据, Lazy加载
     *
     * @param parameters
     * @param subscriber
     */
    @SuppressWarnings("unchecked")
    public void get(Map<String, String> parameters, LifecycleOwner lifecycleOwner, SingleObserver subscriber) {
        apiService.executeGet("", parameters)
                .compose(switchSchedulers())
                .compose(dealWithError())
                .map(new GsonFunctionMap<>(parameters.get("t"), subscriber.getClass()))
                .as(bindLifecycle(lifecycleOwner))
                .subscribe(subscriber);
    }

    /**
     * 回调回来不在主线程，使用在一些相对耗时的主线程操作。
     *
     * @param url
     * @param request
     * @param singleSubscriber
     * @param <T>
     */
    @SuppressWarnings("unchecked")
    public <T> void postOnIoThread(String url, T request, SingleObserver singleSubscriber, LifecycleOwner lifecycleOwner) {
        apiService.executePost(url, new BaseRequestWraper<>(request).getMap())
                .compose(switchSchedulersOnOther())
                .compose(dealWithError())
                .map(new GsonFunctionMap<>(singleSubscriber.getClass()))
                .as(bindLifecycle(lifecycleOwner)).subscribe(singleSubscriber);
    }

    public <T> void post(String url, T request, SingleObserver singleSubscriber, LifecycleOwner lifecycleOwner) {
        post(url, request, singleSubscriber, lifecycleOwner, false);
    }

    @SuppressWarnings("unchecked")
    public <T> void post(String url, T request, SingleObserver singleSubscriber, LifecycleOwner lifecycleOwner, boolean isMainClass) {
        apiService.executePost(url, new BaseRequestWraper<>(request).getMap())
                .compose(switchSchedulers())
                .compose(dealWithError())
                .map(new GsonFunctionMap<>(singleSubscriber.getClass(), isMainClass))
                .as(bindLifecycle(lifecycleOwner)).subscribe(singleSubscriber);
    }


    /**
     * 回调到主线程
     *
     * @param <T>
     * @return
     */
    private static <T> SingleTransformer<T, T> switchSchedulers() {
        return observable -> observable
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread());
    }

    private static <T> SingleTransformer<T, T> switchSchedulersOnOther() {
        return observable -> observable
                .subscribeOn(Schedulers.io())
                .observeOn(Schedulers.io());
    }


    /**
     * 注册错误处理
     *
     * @return
     */
    private static <T> SingleTransformer<Response<T>, T> dealWithError() {
        return upstream -> upstream.map(tResponse -> {
            if (!tResponse.isSuccessful()) {
                assert tResponse.errorBody() != null;
                LogUtils.E(TAG, "response error" + tResponse.errorBody().string());
                throw new ResponseError(tResponse.code(), "服务器错误");
            }
            return tResponse.body();
        });
    }

    private static <T> ObservableTransformer<Response<T>, T> dealWithObservableError() {
        return responseObservable -> responseObservable.map(tResponse -> {
            if (!tResponse.isSuccessful() || tResponse.code() != 200) {
                assert tResponse.errorBody() != null;
                LogUtils.E(TAG, "response error" + tResponse.errorBody().string());
                throw new ResponseError(tResponse.code(), "服务器错误");
            }
            return tResponse.body();
        });
    }

    /**
     * 自动解绑声明周期，防止内存泄露
     *
     * @param lifecycleOwner
     * @param <T>
     * @return
     */
    public static <T> AutoDisposeConverter<T> bindLifecycle(LifecycleOwner lifecycleOwner) {
        return AutoDispose.autoDisposable(
                AndroidLifecycleScopeProvider.from(lifecycleOwner)
        );
    }
}
