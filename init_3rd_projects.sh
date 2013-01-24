#!/bin/sh

export GIT_SSL_NO_VERIFY=true

set -e #bail on error

git submodule init
git submodule sync
git submodule update

cd FFmpegLibrary
cd jni

echo "try to generate freetype2 configure"
cd freetype2
./autogen.sh
cd ..

echo "try to generate fribidi configure"
cd fribidi
autoreconf -ivf
cd ..

echo "try to generate libass configure"
cd libass
autoreconf -ivf
cd ..

echo "try to generate vo-aacenc configure"
cd vo-aacenc
autoreconf -ivf
cd ..

echo "try to generate vo-amrwbenc configure"
cd vo-amrwbenc
autoreconf -ivf
cd ..

echo "Fininshed configure 3rd projects"

