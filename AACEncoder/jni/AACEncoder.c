#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "EasyAACEncoderAPI.h"
#include "com_aacencoder_JNIAACEncode.h"
#include <android/log.h>
#define  LOG_TAG    "VIDEO_AAC"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

EasyAACEncoder_Handle handle = NULL;
//char* jstringTostr(JNIEnv* env, jstring jstr);

/**
 * 返回值 char* 这个代表char数组的首地址
 *  Jstring2CStr 把java中的jstring的类型转化成一个c语言中的char 字符串
 */
/*char* Jstring2CStr(JNIEnv* env, jstring jstr) {
	char* rtn = NULL;
	jclass clsstring = (*env)->FindClass(env, "java/lang/String"); //String
	jstring strencode = (*env)->NewStringUTF(env, "GB2312"); // 得到一个java字符串 "GB2312"
	jmethodID mid = (*env)->GetMethodID(env, clsstring, "getBytes",
			"(Ljava/lang/String;)[B"); //[ String.getBytes("gb2312");
	jbyteArray barr = (jbyteArray) (*env)->CallObjectMethod(env, jstr, mid,
			strencode); // String .getByte("GB2312");
	jsize alen = (*env)->GetArrayLength(env, barr); // byte数组的长度
	jbyte* ba = (*env)->GetByteArrayElements(env, barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char*) malloc(alen + 1); //"\0"
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	(*env)->ReleaseByteArrayElements(env, barr, ba, 0); //
	return rtn;
}*/

char* jstringTostr(JNIEnv* env, jstring jstr) {
	char* pStr = NULL;

	jclass jstrObj = (*env)->FindClass(env, "java/lang/String");
	jstring encode = (*env)->NewStringUTF(env, "utf-8");
	jmethodID methodId = (*env)->GetMethodID(env, jstrObj, "getBytes",
			"(Ljava/lang/String;)[B");
	jbyteArray byteArray = (jbyteArray) (*env)->CallObjectMethod(env, jstr,
			methodId, encode);
	jsize strLen = (*env)->GetArrayLength(env, byteArray);
	jbyte *jBuf = (*env)->GetByteArrayElements(env, byteArray, JNI_FALSE);

	if (jBuf > 0) {
		pStr = (char*) malloc(strLen + 1);

		if (!pStr) {
			return NULL ;
		}

		memcpy(pStr, jBuf, strLen);

		pStr[strLen] = 0;
	}

	(*env)->ReleaseByteArrayElements(env, byteArray, jBuf, 0);

	return pStr;
}

JNIEXPORT void JNICALL Java_com_aacencoder_JNIAACEncode_init(JNIEnv *env,
		jobject obj, jint law) {
	InitParam initParam;
	initParam.u32AudioSamplerate = 8000;
	initParam.ucAudioChannel = 1;
	initParam.u32PCMBitSize = 16;
	if (Law_ALaw == law) {
		initParam.ucAudioCodec = Law_ALaw;
	} else if (Law_ULaw == law) {
		initParam.ucAudioCodec = Law_ULaw;
	} else if (Law_PCM16 == law) {
		initParam.ucAudioCodec = Law_PCM16;
	} else if (Law_G726 == law) {
		initParam.ucAudioCodec = Law_G726;
		initParam.g726param.ucRateBits = Rate40kBits;
	} else {
		LOGE("Java_com_aacencoder_JNIAACEncode_g711Ainit law failure =%d", law);
		return;
	}
	handle = Easy_AACEncoder_Init(initParam);
	LOGE("Java_com_aacencoder_JNIAACEncode_g711Ainit env=%p", env);
}

/*JNIEXPORT void JNICALL Java_com_aacencoder_JNIAACEncode_g711Ainit(JNIEnv *env,
		jobject obj) {

	InitParam initParam;
	initParam.u32AudioSamplerate = 8000;
	initParam.ucAudioChannel = 1;
	initParam.u32PCMBitSize = 16;
	initParam.ucAudioCodec = Law_ALaw;
	//initParam.ucAudioCodec = Law_ULaw;
	handle = Easy_AACEncoder_Init(initParam);
	LOGE("Java_com_aacencoder_JNIAACEncode_g711Ainit env=%p", env);
}


 * Class:     com_aacencoder_JNIAACEncode
 * Method:    g711Uinit
 * Signature: ()V

JNIEXPORT void JNICALL Java_com_aacencoder_JNIAACEncode_g711Uinit(JNIEnv *env,
		jobject obj) {
	InitParam initParam;
	initParam.u32AudioSamplerate = 8000;
	initParam.ucAudioChannel = 1;
	initParam.u32PCMBitSize = 16;
//		initParam.ucAudioCodec = Law_ALaw;
	initParam.ucAudioCodec = Law_ULaw;
	handle = Easy_AACEncoder_Init(initParam);
	LOGW("Java_com_aacencoder_JNIAACEncode_g711Uinit");
}*/

/*
 * Class:     com_aacencoder_JNIAACEncode
 * Method:    release
 * Signature: ()V
 */
//JNIEXPORT void JNICALL Java_com_aacencoder_JNIAACEncode_release(JNIEnv *enc,
//		jobject obj) {
//	if (NULL == handle) {
//		LOGI("Java_com_aacencoder_JNIAACEncode_release: failure");
//		return;
//	}
//	Easy_AACEncoder_Release(handle);
//	LOGW("Java_com_aacencoder_JNIAACEncode_release");
//}

/*
 * Class:     com_aacencoder_JNIAACEncode
 * Method:    encode
 * Signature: ([BI[BI)[B
 */
//JNIEXPORT jbyteArray JNICALL Java_com_aacencoder_JNIAACEncode_encode(
//		JNIEnv *env, jobject obj, jbyteArray jpbG711ABuffer, jint len,
//		jbyteArray jpbAACBuffer) {
//	char *infilename = jstringTostr(env, jinfilename); //类型进行转换
//		char *outAacname = jstringTostr(env, joutAacname); //类型进行转换
//1.打开 wav,MP3文件
//	FILE* fwav = fopen(cwav, "rb");
//	FILE* fmp3 = fopen(cmp3, "wb");
//	char* infilename = "g711.g711a";  //标准
//	char* outAacname = "g711.aac";
//		FILE* fpIn = fopen(infilename, "rb");
//		if (NULL == fpIn) {
//			printf("%s:[%d] open %s file failed\n", __FUNCTION__, __LINE__,infilename);
//			return -1;
//		}
//
//		FILE* fpOut = fopen(outAacname, "wb");
//		if (NULL == fpOut) {
//			printf("%s:[%d] open %s file failed\n", __FUNCTION__, __LINE__,outAacname);
//			return -1;
//		}
//		int gBytesRead = 0;
//		int bG711ABufferSize = 500;
//		int bAACBufferSize = 4 * bG711ABufferSize;  //提供足够大的缓冲区
//		unsigned char *pbG711ABuffer = (unsigned char *) malloc(
//				bG711ABufferSize * sizeof(unsigned char));
//		unsigned char *pbAACBuffer = (unsigned char*) malloc(
//				bAACBufferSize * sizeof(unsigned char));
//		unsigned int out_len = 0;
//
//		// 转换为本地数组
//		jshort *input = (*env)->GetShortArrayElements(env, pbG711ABuffer, NULL);
//		jbyte *pjb = (jbyte *) (*env)->GetByteArrayElements(env, pbG711ABuffer, 0);
//		 jsize jlen = (*env)->GetArrayLength(env, pbG711ABuffer);
//
//		while ((gBytesRead = fread(pbG711ABuffer, 1, bG711ABufferSize, fpIn)) > 0) {
//			if (Easy_AACEncoder_Encode(handle, pbG711ABuffer, gBytesRead, pbAACBuffer, &out_len) > 0) {
//				fwrite(pbAACBuffer, 1, out_len, fpOut);
//			}
//		}
//		free(pbG711ABuffer);
//		free(pbAACBuffer);
//		fclose(fpIn);
//		fclose(fpOut);
//		return 0;
//
//}
//unsigned char *byteArrayToByte(JNIEnv* env, jbyteArray byteArray) {
//	jbyte *pjb = (jbyte *) (*env)->GetByteArrayElements(env, byteArray, 0);
//	jsize jlen = (*env)->GetArrayLength(env, byteArray);
//	unsigned char *byBuf = NULL;
//	int bG711ABufferSize = 500;
//	unsigned char *pbG711ABuffer = NULL;
//
//	if (jlen > 0) {
//		pbG711ABuffer = (unsigned char *) malloc(bG711ABufferSize * sizeof(unsigned char));
//		byBuf = (unsigned char*) malloc(jlen + 1);
//		memcpy(byBuf, pjb, jlen);
//		byBuf[jlen] = '\0';
//	} else {
//		byBuf = (unsigned char*) malloc(1);
//		byBuf[0] = '\0';
//	}
//
//	(*env)->ReleaseByteArrayElements(env, byteArray, pjb, 0);
//	return byBuf;
//}
/*
JNIEXPORT jint JNICALL Java_com_aacencoder_JNIAACEncode_encode(JNIEnv *env,
		jobject obj, jbyteArray jinputBuffer, jstring joutAacname) {
	unsigned char *pbG711ABuffer = byteArrayToByte(env, jinputBuffer);
	char *outAacname = jstringTostr(env, joutAacname); //类型进行转换
	FILE* fpOut = fopen(outAacname, "wb");
	if (NULL == fpOut) {
		printf("%s:[%d] open %s file failed\n", __FUNCTION__, __LINE__,
				outAacname);
		return -1;
	}
	int gBytesRead = 0;
	int bG711ABufferSize = 500;
	int bAACBufferSize = 4 * bG711ABufferSize;  //提供足够大的缓冲区
	unsigned char *pbG711ABuffer = (unsigned char *) malloc(
			bG711ABufferSize * sizeof(unsigned char));
	unsigned char *pbAACBuffer = (unsigned char*) malloc(
			bAACBufferSize * sizeof(unsigned char));
	unsigned int out_len = 0;

	while ((gBytesRead = fread(pbG711ABuffer, 1, bG711ABufferSize, fpIn)) > 0) {
		if (Easy_AACEncoder_Encode(handle, pbG711ABuffer, gBytesRead,
				pbAACBuffer, &out_len) > 0) {
			fwrite(pbAACBuffer, 1, out_len, fpOut);
		}
	}
	free(pbG711ABuffer);
	free(pbAACBuffer);
	fclose(fpOut);
}*/

/*
 * Class:     com_aacencoder_JNIAACEncode
 * Method:    encode
 * Signature: (Ljava/lang/String;Ljava/lang/String;)[B
 */
JNIEXPORT jint JNICALL Java_com_aacencoder_JNIAACEncode_encode(JNIEnv *env,
		jobject obj, jstring jinfilename, jstring joutAacname) {

	char *infilename = jstringTostr(env, jinfilename); //类型进行转换
	char *outAacname = jstringTostr(env, joutAacname); //类型进行转换

	LOGE("inFilename = %s", infilename);
	LOGE("outAacname = %s", outAacname);

	//1.打开 wav,MP3文件
//	FILE* fwav = fopen(cwav, "rb");
//	FILE* fmp3 = fopen(cmp3, "wb");

//	char* infilename = "g711.g711a";  //标准
//	char* outAacname = "g711.aac";

	FILE* fpIn = fopen(infilename, "rb");
	if (NULL == fpIn) {
		printf("%s:[%d] open %s file failed\n", __FUNCTION__, __LINE__,
				infilename);
		return -1;
	}

	FILE* fpOut = fopen(outAacname, "wb");
	if (NULL == fpOut) {
		printf("%s:[%d] open %s file failed\n", __FUNCTION__, __LINE__,
				outAacname);
		return -1;
	}

	int gBytesRead = 0;
	int bG711ABufferSize = 500;
	int bAACBufferSize = 4 * bG711ABufferSize;  //提供足够大的缓冲区
	unsigned char *pbG711ABuffer = (unsigned char *) malloc(
			bG711ABufferSize * sizeof(unsigned char));
	unsigned char *pbAACBuffer = (unsigned char*) malloc(
			bAACBufferSize * sizeof(unsigned char));
	unsigned int out_len = 0;

	while ((gBytesRead = fread(pbG711ABuffer, 1, bG711ABufferSize, fpIn)) > 0) {
		if (Easy_AACEncoder_Encode(handle, pbG711ABuffer, gBytesRead,
				pbAACBuffer, &out_len) > 0) {
			fwrite(pbAACBuffer, 1, out_len, fpOut);
		}
	}
	Easy_AACEncoder_Release(handle);
	free(pbG711ABuffer);
	free(pbAACBuffer);
	fclose(fpIn);
	fclose(fpOut);

	return 0;

}
