package com.aacencoder;

public class JNIAACEncode {
	
	public static final int Law_ULaw = 0;/**< U law */
	public static final int Law_ALaw = 1;/**< A law */
	public static final int Law_PCM16 = 2;/**< 16 bit uniform PCM values. 原始 pcm 数据 */  
	public static final int Law_G726 = 3;/**< G726 */
	
//	public static native void g711Ainit();
//	
//	public static native void g711Uinit();
	
	public static native void init(int audioCodec);

//	public static native void release();

//	public native byte[] encode(byte[] pbG711ABuffer, int len, byte[] pbAACBuffer, int out_len);
	
	public static native int encode(String infilename, String outAacname);
	
	static {
		System.loadLibrary("AACEncode");
	}
}
