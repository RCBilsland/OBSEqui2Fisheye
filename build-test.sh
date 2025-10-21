#!/bin/bash

# Clean previous build
rm -rf build-test
mkdir build-test
cd build-test

# Set up environment variables for OBS paths
export OBS_INCLUDE_DIR="../obs-studio-32.0.1/libobs"
export OBS_LIB_DIR="/Applications/OBS.app/Contents/Frameworks"
export SIMDE_INCLUDE_DIR="../simde-0.8.2"
export OBS_GENERATED_INCLUDE_DIR="."

# Create minimal obsconfig.h
cat > obsconfig.h << 'EOF'
#pragma once
#define OBS_VERSION_MAJOR 32
#define OBS_VERSION_MINOR 0
#define OBS_VERSION_PATCH 1
#define HAVE_STRINGS_H 1
#define HAVE_DLFCN_H 1
#define HAVE_INTTYPES_H 1
#define HAVE_STDINT_H 1
#define HAVE_UNISTD_H 1
#define HAVE_MEMORY_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STRING_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TYPES_H 1
#ifdef __APPLE__
#define __APPLE__ 1
#endif
#define HAVE_ALIGNED_ALLOC 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
#define HAVE_STRTOF 1
#define HAVE_STRTOLD 1
#define HAVE_STRTOIMAX 1
#define HAVE_STRTOUMAX 1
#define OBS_INSTALL_PREFIX "/usr/local"
#define OBS_DATA_PATH "data"
#define OBS_INSTALL_DATA_PATH "data"
#define OBS_PLUGIN_PATH "plugins"
#define OBS_INSTALL_PLUGIN_PATH "plugins"
EOF

# Configure and build
cmake -DCMAKE_BUILD_TYPE=Release -f ../CMakeLists-new.txt ..
make

echo "Build complete. Check build-test/ for the plugin."
