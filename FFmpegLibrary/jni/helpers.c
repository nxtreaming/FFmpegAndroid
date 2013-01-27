/*
 * helpers.c
 * Copyright (c) 2012 Jacek Marchwicki
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <android/log.h>
#include <jni.h>

#include "helpers.h"

#define LOG_LEVEL 2
#define LOG_TAG "AVEngine:helper.c"
#define LOGI(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);}
#define LOGE(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);}
#define LOGW(level, ...) if (level <= LOG_LEVEL) {__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__);}

jfieldID java_get_field(JNIEnv *env, char * class_name, JavaField field) {
	jclass clazz = (*env)->FindClass(env, class_name);
	jfieldID jField = (*env)->GetFieldID(env, clazz, field.name, field.signature);
	(*env)->DeleteLocalRef(env, clazz);
	return jField;
}

jmethodID java_get_method(JNIEnv *env, jclass class, JavaMethod method) {
	return (*env)->GetMethodID(env, class, method.name, method.signature);
}
