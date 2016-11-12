# AACEncode
# Android G711(PCMA/PCMU)、G726、PCM音频转码到AAC
首先先介绍下使用的开源库，当然还是要感谢开源库的作者们为我们提供了方便。

采用EasyAACEncoder 此是EasyDarwin开源流媒体服务团队整理、开发的一款音频转码到AAC的工具库，目前支持G711a/G711u/G726/PCM等音频格式的转码，跨平台，支持Windows/Linux/arm.

### 说明 ###
EasyAACEncoder目前支持的音视频格式：

	/* Audio Codec */
	enum Law
	{
		Law_ULaw	=	0, 		/**< U law */
		Law_ALaw	=	1, 		/**< A law */
		Law_PCM16	=	2, 		/**< 16 bit uniform PCM values. 原始 pcm 数据 */  
		Law_G726	=	3		/**< G726 */
	};
	
	/* Rate Bits */
	enum Rate
	{
		Rate16kBits=2,	/**< 16k bits per second (2 bits per ADPCM sample) */
		Rate24kBits=3,	/**< 24k bits per second (3 bits per ADPCM sample) */
		Rate32kBits=4,	/**< 32k bits per second (4 bits per ADPCM sample) */
		Rate40kBits=5	/**< 40k bits per second (5 bits per ADPCM sample) */
	};

### 更多信息 ###
详情请到：[GitHub地址](https://github.com/EasyDarwin/EasyAACEncoder "GitHub地址")

## 回归正题 ##
具体Android怎么使用该库呢，有以下步骤：

>1.在上面地址下载开源库解压

>2.把解压后的开源库放到项目jni文件夹里

>3.去除无用东西，对有错误的进行剔除与修正

>4.新建个调用c方法的类文件并写好调用方法

>5.新建Android.mk文件与Application.mk文件完成相应配置

>6.编辑打包so即可完成

熟练的大神们分分钟钟就搞定了，像我这样不熟悉的还是多多看看资料先写个小的demo练练手之后在来编译就简单多了。
在此过程中第三步与第四步麻烦是最多的。
现在简单说下
在第三步中要先去掉无用的文件，我直接把非.c源文件与.h头文件的直接剔除如下图我的：

![](http://i.imgur.com/jEuCX1b.png)

该开源库也采用了libfacc如图：

![](http://i.imgur.com/xcRJmZJ.png)

之后如果仍然会有报错的文件，找到报错的所在地再解决，有的需要项目里关联C/C++ build才可以，有的还需要配置环境，具体网上教程有很多，就不在累赘，找到适合自己的就行。
还有就是c的方法与c++的方法有的地方不一样的需要格外注意下比如说env。
对四步主要看你的Java层怎么写方法调用了，然后编译自动生成即可，或者自己编写也行。
Java代码文件方法比如我的：

		public class JNIAACEncode {
	
			public static final int Law_ULaw = 0;/**< U law */
			public static final int Law_ALaw = 1;/**< A law */
			public static final int Law_PCM16 = 2;/**< 16 bit uniform PCM values. 原始 pcm 数据 */  
			public static final int Law_G726 = 3;/**< G726 */
			
			//首先进行实例化audioCodec为上面声明的音频类型
			public static native void init(int audioCodec);
			//原音频文件路径与转码后的音频文件路径
			public static native int encode(String infilename, String outAacname);
			
			static {
				System.loadLibrary("AACEncode");
			}
		}

对应的c代码为：

		JNIEXPORT void JNICALL Java_com_aacencoder_JNIAACEncode_init(JNIEnv *env,jobject obj, jint law) {
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

转码方法为：

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

上面方法别忘了引入头文件：

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

把这些搞定基本完成80%啦，又可以愉快玩耍了。
下面要进行第五步了那就是编写配置文件：
这配置文件很简单，大家应该都会就不多说了，只不过要提醒下LOCAL_SRC_FILES列出所有文件要注意c文件与cpp文件区别。
如下我的：

		LOCAL_PATH := $(call my-dir)

		#生成so动态库
		include $(CLEAR_VARS)
		
		LOCAL_MODULE    := AACEncode
		
		LOCAL_SRC_FILES := \
		./libfaac/aacquant.c \
		./libfaac/backpred.c \
		./libfaac/bitstream.c \
		./libfaac/channels.c \
		./libfaac/fft.c \
		./libfaac/filtbank.c \
		./libfaac/frame.c \
		./libfaac/huffman.c\
		./libfaac/ltp.c \
		./libfaac/midside.c \
		./libfaac/psychkni.c \
		./libfaac/tns.c \
		./libfaac/util.c \
		./libfaac/kiss_fft/kiss_fft.c \
		./libfaac/kiss_fft/kiss_fftr.c \
		audio_buffer.cpp EasyAACEncoder.cpp EasyAACEncoderAPI.cpp g711.cpp G711AToPcm.cpp g726.cpp G726ToPcm.cpp IDecodeToPcm.cpp PcmToAac.cpp AACEncoder.c
		 
		LOCAL_LDLIBS += -llog
		
		
		include $(BUILD_SHARED_LIBRARY)

然后编译运行即可生成so库啦啦啦~
在libs文件里就能找到的~
本人使用的还Eclipse工具编译，由于Android Studio支持不是很好也比较麻烦。
就简单的说这些吧，提醒下对于第三步与第四步问题估计比较多，有问题那就多多去谷歌，问题会解决的！



