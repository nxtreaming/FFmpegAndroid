#ifndef PLAYER_STATIC_H
#define PLAYER_STATIC_H 1

// InterruptedException
static char *interrupted_exception_class_path = "java/lang/InterruptedException";

// RuntimeException
static char *runtime_exception_class_path = "java/lang/RuntimeException";

// NotPlayingException
static char *not_playing_exception_class_path = "net/uplayer/ffmpeg/NotPlayingException";

// HashMap
static char *map_class_path = "java/util/Map";
static JavaMethod map_key_set = {"keySet", "()Ljava/util/Set;"};
static JavaMethod map_get = {"get", "(Ljava/lang/Object;)Ljava/lang/Object;"};

// Set
static char *set_class_path = "java/util/Set";
static JavaMethod set_iterator = {"iterator", "()Ljava/util/Iterator;"};

// Iterator
static char *iterator_class_path = "java/util/Iterator";
static JavaMethod iterator_next = {"next", "()Ljava/lang/Object;"};
static JavaMethod iterator_has_next = {"hasNext", "()Z"};

static const struct {
    const char *name;
    int         nb_channels;
    uint64_t     layout;
} channel_android_layout_map[] = {
    { "mono",        1,  AV_CH_LAYOUT_MONO },
    { "stereo",      2,  AV_CH_LAYOUT_STEREO },
    { "2.1",         3,  AV_CH_LAYOUT_2POINT1 },
    { "4.0",         4,  AV_CH_LAYOUT_4POINT0 },
    { "4.1",         5,  AV_CH_LAYOUT_4POINT1 },
    { "5.1",         6,  AV_CH_LAYOUT_5POINT1_BACK },
    { "6.0",         6,  AV_CH_LAYOUT_6POINT0 },
    { "7.0(front)",  7,  AV_CH_LAYOUT_7POINT0_FRONT },
    { "7.1",         8,  AV_CH_LAYOUT_7POINT1 },
};

// FFmpegPlayer
static JavaField player_mNativePlayer = {"mNativePlayer", "I"};
static JavaMethod player_onUpdateTime = {"onUpdateTime","(IIZ)V"};
static JavaMethod player_prepareAudioTrack = {"prepareAudioTrack", "(II)Landroid/media/AudioTrack;"};
static JavaMethod player_prepareFrame = {"prepareFrame", "(II)Landroid/graphics/Bitmap;"};

// AudioTrack
static char *android_track_class_path = "android/media/AudioTrack";
static JavaMethod audio_track_write = {"write", "([BII)I"};
static JavaMethod audio_track_pause = {"pause", "()V"};
static JavaMethod audio_track_play = {"play", "()V"};
static JavaMethod audio_track_flush = {"flush", "()V"};
static JavaMethod audio_track_stop = {"stop", "()V"};
static JavaMethod audio_track_getChannelCount = {"getChannelCount", "()I"};
static JavaMethod audio_track_getSampleRate = {"getSampleRate", "()I"};

#endif
