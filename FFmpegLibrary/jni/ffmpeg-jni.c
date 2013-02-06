/*
 * ffmpeg-jni.c
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

#include <jni.h>
#include <android/log.h>
/*standard library*/
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <unistd.h>
#include <assert.h>

#include "helpers.h"
#include "player.h"

#define LOG_TAG "AVEngine:ffmpeg-jni.c"
#define LOG_LEVEL 10

static JNINativeMethod player_methods[] = {
	{"initNative", "()I", (void*) jni_player_init},
	{"deallocNative", "()V", (void*) jni_player_dealloc},
	{"seekNative", "(I)V", (void*) jni_player_seek},
	{"pauseNative", "()V", (void*) jni_player_pause},
	{"resumeNative", "()V", (void*) jni_player_resume},
	{"setDataSourceNative", "(Ljava/lang/String;Ljava/util/Map;III)I", (void*) jni_player_set_data_source},
	{"stopNative", "()V", (void*) jni_player_stop},
	{"renderFrameStart", "()V", (void*) jni_player_render_frame_start},
	{"renderFrameStop", "()V", (void*) jni_player_render_frame_stop},
	{"renderFrameNative", "()Landroid/graphics/Bitmap;", (void*) jni_player_render_frame},
	{"releaseFrame", "()V", (void*) jni_player_release_frame},
	{"getVideoDurationNative", "()I", (void*) jni_player_get_video_duration},
};

static int register_native_methods(JNIEnv* env,
		const char* class_name,
		JNINativeMethod* methods,
		int num_methods)
{
	jclass clazz;

	clazz = (*env)->FindClass(env, class_name);
	if (clazz == NULL) {
		fprintf(stderr, "Native registration unable to find class '%s'\n",
				class_name);
		return JNI_FALSE;
	}
	if ((*env)->RegisterNatives(env, clazz, methods, num_methods) < 0) {
		fprintf(stderr, "RegisterNatives failed for '%s'\n", class_name);
		return JNI_FALSE;
	}

	return JNI_TRUE;
}

jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

	if ((*vm)->GetEnv(vm, (void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		fprintf(stderr, "ERROR: GetEnv failed\n");
		goto bail;
	}
	assert(env != NULL);

	if (register_native_methods(env,
			player_class_path,
			player_methods,
			NELEM(player_methods)) < 0) {
		fprintf(stderr, "ERROR: Exif native registration failed\n");
		goto bail;
	}

	/* success -- return valid version number */
	result = JNI_VERSION_1_4;

bail:
	return result;
}

void JNI_OnUnload(JavaVM *vm, void *reserved)
{
}
