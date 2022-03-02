package me.simple.lib.net;


import com.google.gson.JsonElement;

import java.util.List;
import java.util.Map;

import io.reactivex.Observable;
import io.reactivex.Single;
import okhttp3.MultipartBody;
import okhttp3.ResponseBody;
import retrofit2.Response;
import retrofit2.http.FieldMap;
import retrofit2.http.FormUrlEncoded;
import retrofit2.http.GET;
import retrofit2.http.Header;
import retrofit2.http.Multipart;
import retrofit2.http.POST;
import retrofit2.http.Part;
import retrofit2.http.QueryMap;
import retrofit2.http.Streaming;
import retrofit2.http.Url;

public interface ApiService {

    @GET
    Single<Response<JsonElement>> executeGet(
            @Url String url,
            @QueryMap Map<String, String> maps);

//    @GET
//    Observable<Response<JsonElement>> executeGet(
//            @Url String url,
//            @QueryMap Map<String, String> maps);

    @POST
    @FormUrlEncoded
    Single<Response<JsonElement>> executePost(
            @Url String url,
            @FieldMap Map<String, String> maps);


    @POST
    @FormUrlEncoded
    Observable<Response<JsonElement>> executeObservablePost(
            @Url String url,
            @FieldMap Map<String, String> maps);

    @Streaming
    @GET
    Observable<ResponseBody> download(@Header("RANGE") String start, @Url String url);

    @Streaming
    @GET
    Observable<ResponseBody> download(@Url String url);

    @Multipart
    @POST()
    Single<Response<JsonElement>> imageUpload(@Url String url, @Part() List<MultipartBody.Part> imgs, @Part("data") String requestBody);

}
