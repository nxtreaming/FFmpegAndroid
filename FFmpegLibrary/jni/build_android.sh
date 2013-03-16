#!/bin/bash
#
# build_android.sh
# Copyright (c) 2012 Jacek Marchwicki
# Copyright (c) 2013 GoogleGeek
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

OS=`uname -s | tr '[A-Z]' '[a-z]'`

#CFLAGS="-I$ARM_INC -fpic -DANDROID -fpic -mthumb-interwork -ffunction-sections -funwind-tables -fstack-protector -fno-short-enums -D__ARM_ARCH_5__ -D__ARM_ARCH_5T__ -D__ARM_ARCH_5E__ -D__ARM_ARCH_5TE__  -Wno-psabi -march=armv5te -mtune=xscale -msoft-float -mthumb -Os -fomit-frame-pointer -fno-strict-aliasing -finline-limit=64 -DANDROID  -Wa,--noexecstack -MMD -MP"

SYSROOT=${NDK}/platforms/android-9/arch-arm
CROSS_PREFIX=${NDK}/toolchains/arm-linux-androideabi-4.6/prebuilt/${OS}-x86/bin/arm-linux-androideabi-
PREFIX_BASE=../ffmpeg_build

function build_x264()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building x264..."
	cd x264

	./configure \
	    --cross-prefix=${CROSS_PREFIX} \
	    --sysroot=${SYSROOT} \
	    --prefix=$(pwd)/$PREFIX \
	    --host=arm-linux \
	    --enable-static \
	    --enable-strip \
	    --disable-cli \
	    --disable-avs \
	    --disable-swscale \
	    --disable-lavf \
	    --disable-ffms \
	    --disable-gpac \
	    --disable-interlaced \
	    --chroma-format=420 \
	    --enable-pic \
	    --extra-cflags="-D_ANDROID_SYS_ -fno-tree-vectorize -mvectorize-with-neon-quad -funsafe-math-optimizations ${CFLAGS}" \
	    --extra-ldflags="-Wl,-rpath-link=${SYSROOT}/usr/lib -L${SYSROOT}/usr/lib" \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_amrwb()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building amrwbenc..."
	cd vo-amrwbenc

	./configure \
	    --host=arm-linux \
	    --disable-dependency-tracking \
	    --disable-shared \
	    --enable-static \
	    --with-pic \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_voaac()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building vo-aacenc..."
	cd vo-aacenc

	export PKG_CONFIG_LIBDIR=$(pwd)/$PREFIX/lib/pkgconfig/
	export PKG_CONFIG_PATH=$(pwd)/$PREFIX/lib/pkgconfig/
	./configure \
	    --prefix=$(pwd)/$PREFIX \
	    --host=arm-linux \
	    --disable-dependency-tracking \
	    --disable-shared \
	    --enable-static \
	    --with-pic \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_freetype2()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building freetype2..."
	cd freetype2

	export PKG_CONFIG_LIBDIR=$(pwd)/$PREFIX/lib/pkgconfig/
	export PKG_CONFIG_PATH=$(pwd)/$PREFIX/lib/pkgconfig/
	./configure \
	    --prefix=$(pwd)/$PREFIX \
	    --host=arm-linux \
	    --disable-dependency-tracking \
	    --disable-shared \
	    --enable-static \
	    --with-pic \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_ass()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building libass..."
	cd libass

	export PKG_CONFIG_LIBDIR=$(pwd)/$PREFIX/lib/pkgconfig/
	export PKG_CONFIG_PATH=$(pwd)/$PREFIX/lib/pkgconfig/
	./configure \
	    --prefix=$(pwd)/$PREFIX \
	    --host=arm-linux \
	    --disable-fontconfig \
	    --disable-dependency-tracking \
	    --disable-shared \
	    --enable-static \
	    --with-pic \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_fribidi()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building fribidi..."
	cd fribidi

	./configure \
	    --prefix=$(pwd)/$PREFIX \
	    --host=arm-linux \
	    --disable-dependency-tracking \
	    --disable-shared \
	    --enable-static \
	    --with-pic \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_fdkaac()
{
	LOCAL_CFLAGS=$1
	PREFIX=$2

	echo "Building fdk-aac..."
	cd fdk-aac

	./configure \
	    --host=arm-linux \
	    --with-sysroot=${SYSROOT} \
	    --prefix=$(pwd)/${PREFIX} \
	    --disable-shared \
	    --enable-static \
	    --with-pic=no \
	    CC="${CROSS_PREFIX}gcc --sysroot=${SYSROOT}" \
	    CXX="${CROSS_PREFIX}g++ --sysroot=${SYSROOT}" \
	    RANLIB="${CROSS_PREFIX}ranlib" \
	    AR="${CROSS_PREFIX}ar" \
	    AR_FLAGS=rcu \
	    STRIP="${CROSS_PREFIX}strip" \
	    NM="${CROSS_PREFIX}nm" \
	    CFLAGS="-O3 ${LOCAL_CFLAGS} --sysroot=${SYSROOT}" \
	    CXXFLAGS="-O3 ${LOCAL_CFLAGS} --sysroot=${SYSROOT}" \
	    || exit 1

	    make clean || exit 1
	    make V=1 -j4 install || exit 1

	    cd ..
}

function build_ffmpeg()
{
	CFLAGS=$1
	PREFIX=$2

	echo "Building FFmpeg..."
	cd ffmpeg

	export PKG_CONFIG_LIBDIR=$(pwd)/${PREFIX}/lib/pkgconfig/
	export PKG_CONFIG_PATH=$(pwd)/${PREFIX}/lib/pkgconfig/
	./configure \
	    --target-os=linux \
	    --sysroot=${SYSROOT} \
	    --enable-cross-compile \
	    --cross-prefix=$CROSS_PREFIX \
	    --prefix=$(pwd)/$PREFIX \
	    --arch=armv7-a \
	    --extra-cflags="-fpic -DANDROID -DHAVE_SYS_UIO_H=1 -Dipv6mr_interface=ipv6mr_ifindex -fasm -Wno-psabi -fno-short-enums -fno-strict-aliasing -finline-limit=300 $CFLAGS -I${PREFIX}/include" \
	    --extra-ldflags="-Wl,-rpath-link=${SYSROOT}/usr/lib -L${SYSROOT}/usr/lib -nostdlib -L${PREFIX}/lib" \
	    --extra-libs="-llog -lc -lm -ldl -lgcc -lz" \
	    --disable-shared \
	    --enable-static \
	    --enable-pic \
	    --enable-runtime-cpudetect \
	    --enable-asm \
	    --enable-armv5te \
	    --enable-armv6 \
	    --enable-armv6t2 \
	    --enable-vfp \
	    --enable-neon \
	    --enable-thumb \
	    --disable-avdevice \
	    --disable-avfilter \
	    --disable-avresample \
	    --disable-postproc \
	    --disable-swscale-alpha \
	    --disable-dct \
	    --disable-dwt \
	    --disable-lsp \
	    --disable-lzo \
	    --disable-mdct \
	    --disable-rdft \
	    --disable-fft \
	    --disable-everything \
	    --enable-libfdk-aac \
	    --enable-decoder=libfdk_aac \
	    --enable-encoder=libfdk_aac \
	    --enable-libx264 \
	    --enable-encoder=libx264 \
	    --enable-demuxer=flv \
	    --enable-demuxer=mov \
	    --enable-demuxer=hls \
	    --enable-demuxer=mpegts \
	    --enable-muxer=flv \
	    --enable-protocol=file \
	    --enable-protocol=tcp \
	    --enable-protocol=hls \
	    --enable-protocol=http \
	    --enable-decoder=h264 \
	    --enable-parser=h264 \
	    --enable-parser=aac \
	    --enable-zlib \
	    --disable-doc \
	    --disable-programs \
	    --enable-gpl \
	    --enable-nonfree \
	    --enable-version3 \
	    --enable-memalign-hack \
	    || exit 1

	make clean || exit 1
	make V=1 -j4 install || exit 1

	cd ..
}

function build_one()
{
	SONAME=$1
	PREFIX=$2

	echo "Building one..."
	cd ffmpeg

	${CROSS_PREFIX}ld -rpath-link=${SYSROOT}/usr/lib -L${SYSROOT}/usr/lib -L${PREFIX}/lib -soname $SONAME -shared -nostdlib -z noexecstack -Bsymbolic --whole-archive --no-undefined -o ${PREFIX}/${SONAME} -lavformat -lavcodec -lswresample -lswscale -lavutil -lx264 -lfdk-aac -lc -lm -lz -ldl -llog --dynamic-linker=/system/bin/linker -zmuldefs ${NDK}/toolchains/arm-linux-androideabi-4.6/prebuilt/${OS}-x86/lib/gcc/arm-linux-androideabi/4.6/libgcc.a || exit 1

	cd ..
}

#arm v7vfpv3
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=vfpv3-d16 -marm -march=armv7-a -mthumb -D__thumb__"
PREFIX=${PREFIX_BASE}/armeabi-v7a
SONAME=libffmpeg.so
build_fdkaac "$OPTIMIZE_CFLAGS" "$PREFIX"
build_x264 "$OPTIMIZE_CFLAGS" "$PREFIX"
build_ffmpeg "$OPTIMIZE_CFLAGS" "$PREFIX"
build_one "$SONAME" "$PREFIX"

#arm v7 + neon
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=neon -marm -march=armv7-a -mtune=cortex-a8 -mthumb -D__thumb__"
PREFIX=${PREFIX_BASE}/armeabi-v7a-neon
SONAME=libffmpeg-neon.so
build_fdkaac "$OPTIMIZE_CFLAGS" "$PREFIX"
build_x264 "$OPTIMIZE_CFLAGS" "$PREFIX"
build_ffmpeg "$OPTIMIZE_CFLAGS" "$PREFIX"
build_one "$SONAME" "$PREFIX"

