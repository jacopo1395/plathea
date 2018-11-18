/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class io_core_InterfaceJNI */

#ifndef _Included_io_core_InterfaceJNI
#define _Included_io_core_InterfaceJNI
#ifdef __cplusplus
extern "C" {
#endif
	/*
	* Class:     io_core_InterfaceJNI
	* Method:    loadConfigurationFile
	* Signature: (Ljava/lang/String;)I
	*/
	JNIEXPORT jint JNICALL Java_io_core_InterfaceJNI_loadConfigurationFile
	(JNIEnv *, jobject, jstring);

	/*
	* Class:     io_core_InterfaceJNI
	* Method:    internalCalibration
	* Signature: (Ljava/lang/String;I)I
	*/
	JNIEXPORT jint JNICALL Java_io_core_InterfaceJNI_internalCalibration
	(JNIEnv *, jobject, jstring, jint);

	/*
	* Class:     io_core_InterfaceJNI
	* Method:    externalCalibration
	* Signature: (Ljava/lang/String;)I
	*/
	JNIEXPORT jint JNICALL Java_io_core_InterfaceJNI_externalCalibration
	(JNIEnv *, jobject, jstring);

	/*
	* Class:     io_core_InterfaceJNI
	* Method:    selectSVMclassifier
	* Signature: (Ljava/lang/String;)I
	*/
	JNIEXPORT jint JNICALL Java_io_core_InterfaceJNI_selectSVMclassifier
	(JNIEnv *, jobject, jstring);

	/*
	* Class:     io_core_InterfaceJNI
	* Method:    initializeSystem
	* Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;Ljava/lang/String;ILjava/lang/String;I)I
	*/
	JNIEXPORT jint JNICALL Java_io_core_InterfaceJNI_initializeSystem
	(JNIEnv *, jobject, jstring, jstring, jstring, jstring, jint, jstring, jstring, jint, jstring, jint);

#ifdef __cplusplus
}
#endif
#endif
