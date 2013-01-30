/*
 * player.h
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

#ifndef H_PLAYER
#define H_PLAYER

#include <libavutil/audioconvert.h>

static char *player_class_path = "net/uplayer/ffmpeg/FFmpegPlayer";

enum Msgs {
	MSG_NONE = 0, MSG_STOP = 1
};

enum PlayerErrors {
	ERROR_NO_ERROR = 0,

	// Java errors
	ERROR_NOT_FOUND_PLAYER_CLASS,
	ERROR_NOT_FOUND_PREPARE_FRAME_METHOD,
	ERROR_NOT_FOUND_ON_UPDATE_TIME_METHOD,
	ERROR_NOT_FOUND_PREPARE_AUDIO_TRACK_METHOD,
	ERROR_NOT_FOUND_SET_STREAM_INFO_METHOD,
	ERROR_NOT_FOUND_M_NATIVE_PLAYER_FIELD,
	ERROR_COULD_NOT_GET_JAVA_VM,
	ERROR_COULD_NOT_DETACH_THREAD,
	ERROR_COULD_NOT_ATTACH_THREAD,
	ERROR_COULD_NOT_CREATE_GLOBAL_REF_FOR_AUDIO_TRACK_CLASS,

	// AudioTrack
	ERROR_NOT_FOUND_AUDIO_TRACK_CLASS,
	ERROR_NOT_FOUND_WRITE_METHOD,
	ERROR_NOT_FOUND_PLAY_METHOD,
	ERROR_NOT_FOUND_PAUSE_METHOD,
	ERROR_NOT_FOUND_STOP_METHOD,
	ERROR_NOT_FOUND_RELEASE_METHOD,
	ERROR_NOT_FOUND_GET_CHANNEL_COUNT_METHOD,
	ERROR_NOT_FOUND_FLUSH_METHOD,
	ERROR_NOT_FOUND_GET_SAMPLE_RATE_METHOD,

	ERROR_COULD_NOT_CREATE_AVCONTEXT,
	ERROR_COULD_NOT_OPEN_VIDEO_FILE,
	ERROR_COULD_NOT_OPEN_STREAM,
	ERROR_COULD_NOT_OPEN_VIDEO_STREAM,
	ERROR_COULD_NOT_FIND_VIDEO_CODEC,
	ERROR_COULD_NOT_OPEN_VIDEO_CODEC,
	ERROR_COULD_NOT_ALLOC_FRAME,

	ERROR_NOT_CREATED_BITMAP,
	ERROR_COULD_NOT_GET_SWS_CONTEXT,
	ERROR_COULD_NOT_PREPARE_PACKETS_QUEUE,
	ERROR_COULD_NOT_FIND_AUDIO_STREAM,
	ERROR_COULD_NOT_FIND_AUDIO_CODEC,
	ERROR_COULD_NOT_OPEN_AUDIO_CODEC,
	ERROR_COULD_NOT_PREPARE_RGB_QUEUE,
	ERROR_COULD_NOT_PREPARE_AUDIO_PACKETS_QUEUE,
	ERROR_COULD_NOT_PREPARE_VIDEO_PACKETS_QUEUE,

	ERROR_WHILE_DUPLICATING_FRAME,

	ERROR_WHILE_DECODING_VIDEO,
	ERROR_COULD_NOT_RESAMPLE_FRAME,
	ERROR_WHILE_ALLOCATING_AUDIO_SAMPLE,
	ERROR_WHILE_DECODING_AUDIO_FRAME,
	ERROR_NOT_CREATED_AUDIO_TRACK,
	ERROR_NOT_CREATED_AUDIO_TRACK_GLOBAL_REFERENCE,
	ERROR_COULD_NOT_INIT_SWR_CONTEXT,
	ERROR_NOT_CREATED_AUDIO_SAMPLE_BYTE_ARRAY,
	ERROR_PLAYING_AUDIO,
	ERROR_WHILE_LOCING_BITMAP,

	ERROR_COULD_NOT_JOIN_PTHREAD,
	ERROR_COULD_NOT_INIT_PTHREAD_ATTR,
	ERROR_COULD_NOT_CREATE_PTHREAD,
	ERROR_COULD_NOT_DESTROY_PTHREAD_ATTR,
	ERROR_COULD_NOT_ALLOCATE_MEMORY,

	ERROR_NOT_STOP_LAST_INSTANCE,
};

enum DecodeCheckMsg {
	DECODE_CHECK_MSG_STOP = 0, DECODE_CHECK_MSG_FLUSH,
};

enum ReadFromStreamCheckMsg {
	READ_FROM_STREAM_CHECK_MSG_STOP = 0, READ_FROM_STREAM_CHECK_MSG_SEEK,
};

enum RenderCheckMsg {
	RENDER_CHECK_MSG_INTERRUPT = 0, RENDER_CHECK_MSG_FLUSH,
};

// Player
int jni_player_init(JNIEnv *env, jobject thiz);
void jni_player_dealloc(JNIEnv *env, jobject thiz);
void jni_player_seek(JNIEnv *env, jobject thiz, jint position);
void jni_player_pause(JNIEnv *env, jobject thiz);
void jni_player_resume(JNIEnv *env, jobject thiz);
int jni_player_set_data_source(JNIEnv *env, jobject thiz, jstring string,
	jobject dictionary, int video_index, int audio_index, int subtitle_index);
void jni_player_stop(JNIEnv *env, jobject thiz);
void jni_player_render_frame_start(JNIEnv *env, jobject thiz);
void jni_player_render_frame_stop(JNIEnv *env, jobject thiz);
jobject jni_player_render_frame(JNIEnv *env, jobject thiz);
void jni_player_release_frame (JNIEnv *env, jobject thiz);
int jni_player_get_video_duration(JNIEnv *env, jobject thiz);

#endif
