#include <cstdio>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stdio.h>

#define SIZE 256

#include <string>
#include "string.h"

const char baseData[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";


char *strstrstr(const register char *str, const register char *front, const register char *rear,
                bool isStatic) {
    static char StaticStr[512];
    if (!str)
        return NULL;

    const register char *one;
    const register char *two;
    const register char *f;
    const register char *r;

    if (front) //如果第一个查找参数为空则跳过查找
    {
        do {
            if (*str == '\0') //如果被查找参数结束则返回NULL
                return NULL;
            one = str;           //给第一个寄存器赋值为被查找参数
            str++;               //被查找参数向前推进
            f = front;           //给寄存器赋值为第一个查找参数
            while (*one == *f) //如果字符匹配则继续匹配下一个
            {
                if (*f == '\0') //如果第一个查找参数结束并且被查找字符串结束则查找失败
                    return NULL;
                one++; //继续匹配下一个
                f++;
            }
        } while (*f); //查找结束
        //此处one为截取结果头部指针
        str = one; //被查找参数移动到第一个查找参数后面
    } else
        one = str; //如果第一个查找参数为空则跳过查找

    if (rear == NULL || *rear == '\0')
        while (*str)
            str++;
    else {
        do {
            if (*str == '\0')
                return NULL;
            two = str;
            str++;
            r = rear;
            while (*two == *r) {
                if (*r == '\0')
                    break;
                two++;
                r++;
            }
        } while (*r);
        str--;
    }

    two = str;

    //two = strstr(one,rear);
    int onelen = two - one;
    char *newstr;
    if (isStatic)
        newstr = StaticStr;
    else if ((newstr = (char *) malloc(onelen + 1)) == NULL)
        return NULL;
    strncpy(newstr, one, onelen);
    newstr[onelen] = '\0';
    return newstr;
}
//strstr
void OooOoOOoOOoOo(const char *s, char ch1, char ch2, char *substr)    // 分割字符串
{
    while (*s && *s++ != ch1);
    while (*s && *s != ch2)*substr++ = *s++;
    *substr = '\0';
}

//get_index
char OooOoOOoOoo(char ch) {
    char *ptr = (char *) strrchr(baseData, ch);
    return (ptr - baseData);
}


//rc4
void OooOoOOoOOoOO(unsigned char *pSrc, int nSrcLen, const char *pKey, int nKeyLen) {
    const int MAX_SIZE = 10 * 1024;
    unsigned char S[256] = {0}, T[256] =
            {
                    0}, R[MAX_SIZE] =
            {
                    0};

    if (!pSrc || !pKey || nSrcLen <= 0 || nSrcLen > MAX_SIZE || nKeyLen <= 0)
        return;

    int i = 0;
    for (i = 0; i < 256; i++) {
        S[i] = i;
        T[i] = pKey[i % nKeyLen];
    }

    int j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;

        unsigned char temp = S[i];
        S[i] = S[j], S[j] = temp;
    }

    i = 0, j = 0;
    for (int m = 0; m < MAX_SIZE; m++) {
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;

        unsigned char temp = S[i];
        S[i] = S[j], S[j] = temp;

        int t = (S[i] + S[j]) % 256;
        R[m] = S[t];
    }

    for (i = 0; i < nSrcLen; i++) {
        pSrc[i] ^= R[i];
    }
}

//DecodeString2Binary
void OOoOOoOooOooO(const char *src, int lenSrc, char *&res, int &lenRes) {
    lenRes = lenSrc * 3 / 4;
    res = new char[lenRes];

    int cp = 0;
    if (src[lenSrc - 1] == '=') {
        lenRes--, cp++;
    }
    if (src[lenSrc - 2] == '=') {
        lenRes--, cp++;
    }

    char *newSrc = new char[lenSrc - cp];
    int i;
    for (i = 0; i < lenSrc - cp; i++)
        newSrc[i] = OooOoOOoOoo(src[i]);

    for (i = 0; i < lenRes; i += 3) {
        res[i] = (newSrc[i / 3 * 4] << 2) + (newSrc[i / 3 * 4 + 1] >> 4);
        res[i + 1] = ((newSrc[i / 3 * 4 + 1] & 15) << 4) + (newSrc[i / 3 * 4 + 2] >> 2);
        res[i + 2] = ((newSrc[i / 3 * 4 + 2] & 3) << 6) + (newSrc[i / 3 * 4 + 3]);
    }

    delete[]newSrc;
    return;
}

//DecodeBase64RC4
int OooOooOooOooOo(const char *pSrc, char *pDst, const char *pKey) {
    if (!pSrc || !pDst || !pKey)
        return 0;

    int nSrcLen = strlen(pSrc);
    int nKeyLen = strlen(pKey);

    int nResLen = 0;
    char *pRes = NULL;
    OOoOOoOooOooO(pSrc, nSrcLen, pRes, nResLen);

    if (pRes) {
        OooOoOOoOOoOO((unsigned char *) pRes, nResLen, pKey, nKeyLen);

        memcpy(pDst, pRes, nResLen);
        pDst[nResLen] = '\0';

        delete[]pRes, pRes = NULL;
        return nResLen;
    }
    return 0;
}

using namespace std;


char *strupr(char *str) {
    char *orign = str;
    for (; *str != '\0'; str++)
        *str = toupper(*str);
    return orign;
}

char *binToHex(char *data) {
    std::ostringstream strHex;
    int size = strlen(data);
    strHex << std::hex << std::setfill('0');
    for (int i = 0; i < size; ++i) {
        strHex << std::setw(2) << static_cast < unsigned int >(data[i]);
    }
    std::string str = strHex.str();
    return strupr(const_cast<char *>(str.c_str()));
}

char *hexToBin(const std::string &hex) {
    std::vector<unsigned char> dest;
    unsigned int len = hex.size();
    dest.reserve(hex.size() / 2);
    for (int i = 0; i < len; i += 2) {
        unsigned int element;
        std::istringstream strHex(hex.substr(i, 2));
        strHex >> std::hex >> element;
        dest.push_back(static_cast <unsigned char>(element));
    }
    return (char *) dest.data();
}

/*
char* rc4_encryption(char* data, char* key) {
	data = RC4Base(data, key, strlen(data));
	char* mw = binToHex(data);
	return mw;
}


char* rc4_decrypt(const char* d, const char* key) {
	char* mw = hexToBin(d);
	char *data = RC4Base(mw, key, strlen(mw));
	return data;
}

*/
char __to_hex(int i) {
    if (i >= 0 && i <= 9)
        return i + '0';
    else
        return i - 10 + 'a';
}

int __un_hex(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    else
        return c - 'a' + 10;
}

// str是原来的字符串，len是字符串的长度，hex_str是转化后的字符串，且长度是len的两倍
void byte_2_hex(char str[], int len, char hex_str[]) {
    int i, t;
    for (i = 0; i < len; i++) {
        t = str[i] >> 4;
        hex_str[2 * i] = __to_hex(t);
        t = str[i] & 0xf;
        hex_str[2 * i + 1] = __to_hex(t);
    }
}

// hex_str是十六进制的字符串，str是转化后的字符串，len是str的长度，且是hex_str长度的一半
void hex_2_byte(char hex_str[], char str[], int len) {
    int i, t;
    for (i = 0; i < len; i++) {
        t = __un_hex(hex_str[2 * i]);
        str[i] = t << 4;
        t = __un_hex(hex_str[2 * i + 1]);
        str[i] += t;
    }
}


// 初始化密钥
void _init_key(char key[], int key_len, char key_state[SIZE]) {
    int i, j;
    char tmp;

    for (i = 0; i < SIZE; i++)
        key_state[i] = i;
    for (i = j = 0; i < SIZE; i++) {
        j = (j + key_state[i] + key[i % key_len]) % SIZE;
        tmp = key_state[i];
        key_state[i] = key_state[j];
        key_state[j] = tmp;
    }
}

// data是原文，stream是密文，假设两者长度相等
void _rc4_streamming(char key[], int key_len, char data[], int data_len, char stream[]) {
    int i, j, k, xor_index;
    char tmp;

    char *ks = (char *) malloc(SIZE);
    _init_key(key, key_len, ks);

    for (i = j = k = 0; k < data_len; k++) {
        i = (i + 1) % SIZE;
        j = (j + ks[i]) % SIZE;
        tmp = ks[i];
        ks[i] = ks[j];
        ks[j] = tmp;
        xor_index = (ks[i] + ks[j]) % SIZE;
        stream[k] = data[k] ^ ks[xor_index];
    }

    free(ks);
    ks = NULL;
}


// 加密，输入原字节，输出十六进制（返回的指针用完需要free）
char *rc4_encryption(char data[], char key[]) {
    int data_len = strlen(data);
    int hex_data_len = data_len << 1;
    char *hex_data = (char *) malloc(hex_data_len);
    char *stream = (char *) malloc(data_len);
    _rc4_streamming(key, strlen(key), data, data_len, stream);
    byte_2_hex(stream, data_len, hex_data);
    free(stream);
    stream = NULL;
    return hex_data;
}

// 解密，输入十六进制，输出原字节（返回的指针用完需要free）
char *rc4_decrypt(char hex_data[], char key[]) {
    int hex_data_len = strlen(hex_data);
    int data_len = hex_data_len >> 1;
    char *data = (char *) malloc(data_len);
    char *stream = (char *) malloc(data_len);
    hex_2_byte(hex_data, data, data_len);
    _rc4_streamming(key, strlen(key), data, data_len, stream);
    free(data);
    data = NULL;
    return stream;
}



//rc4加解密

void swap(unsigned char *s1, unsigned char *s2) {
    char temp;
    temp = *s1;
    *s1 = *s2;
    *s2 = temp;
}

void re_S(unsigned char *S) {
    unsigned int i;
    for (i = 0; i < 256; i++)
        S[i] = i;
}

void re_T(unsigned char *T, const char *key) {
    int i;
    int keylen;
    keylen = strlen(key);
    for (i = 0; i < 256; i++)
        T[i] = key[i % keylen];
}

void re_Sbox(unsigned char *S, unsigned char *T) {
    int i;
    int j = 0;
    for (i = 0; i < 256; i++) {
        j = (j + S[i] + T[i]) % 256;
        swap(&S[i], &S[j]);
    }
}

void re_RC4(unsigned char *S, const char *key) {
    unsigned char T[255];
    re_S(S);
    re_T(T, key);
    re_Sbox(S, T);
}

//RC4
char *OooOO0o0O0oO0(const char *data, size_t len, const char *key) {
    unsigned char S[255] = {0};
    unsigned char hx;
    int i, j, t;
    re_RC4(S, key);
    i = j = 0;
    const char *mw = data;
    char *zw = (char *) malloc(len + 1);
    if (!zw)
        return NULL;
    char *zwzz = zw;
    while (len) {
        hx = *mw;
        i = (i + 1) % 256;
        j = (j + S[i]) % 256;
        swap(&S[i], &S[j]);
        t = (S[i] + (S[j] % 256)) % 256;
        hx = hx ^ S[t];
        *zwzz = hx;
        mw++;
        zwzz++;
        len--;
    }
    *zwzz = '\0';
    return zw;
}

void OooOoOoo( char key[1024],char* time,char* jc1,char* jc2,char* jc3){
    //char key[1024];
    memset(key, '\0', sizeof(key));
    strcpy(key, time);
    strcat(key, jc1);
    strcat(key, time);
    strcat(key, jc2);
    strcat(key, time);
    strcat(key, jc3);
}
void OooOoOoo( char key[1024],char key2[1024], char* time){
    strcpy(key2, key);
    strcat(key2, time);
}

string decryption;

//rc4解密
void OoOoOooOoOOo(char *jc3, string value5, string value4, char *jc1, char *jc2, char *data,
                  char breakUp1, char breakUp2, char encode,
                  string value, string value1, string value2, string value3) {
    char res2[1024] = {0};
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%M", localtime(&t));
    char resdata[2048];
    OooOoOOoOOoOo(data, breakUp1, breakUp2, resdata);
    // LOGD("获取未解密数据:%s\n", data);
    char key[1024];
    OooOoOoo( key,tmp, jc1, jc2, jc3);
    char key2[1024];
    OooOoOoo( key,key2,tmp);
    OooOooOooOooOo(resdata, res2, key2);
    char res3[1024] = {0};
    OooOooOooOooOo(res2, res3, key);
   // LOGD("res2:%s\n",res2);
    if (res3[0] != encode) {
        int min = atoi(tmp);
        sprintf(tmp, "%d", min - 1);
        OooOoOoo( key,tmp, jc1, jc2, jc3);
        OooOooOooOooOo(resdata, res3, key);
    }
    //  LOGD("密钥为:%s\n",key);
    if (res3[0] != encode) {
        //  LOGD("nmsl破解:解密异常\n");
        exit(0);
    }
    //LOGD("res3:%s\n",res3);
    decryption = (char*)res3 + 1;

}

