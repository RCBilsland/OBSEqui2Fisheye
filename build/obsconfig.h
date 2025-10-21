#pragma once

// Minimal OBS config for plugin development
#define OBS_VERSION_MAJOR 32
#define OBS_VERSION_MINOR 0
#define OBS_VERSION_PATCH 1

// Basic feature flags
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

// Platform specific
#ifdef __APPLE__
#define __APPLE__ 1
#endif

// Compiler features
#define HAVE_ALIGNED_ALLOC 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOULL 1
#define HAVE_STRTOF 1
#define HAVE_STRTOLD 1
#define HAVE_STRTOIMAX 1
#define HAVE_STRTOUMAX 1

// OBS specific
#define OBS_INSTALL_PREFIX "/usr/local"
#define OBS_DATA_PATH "data"
#define OBS_INSTALL_DATA_PATH "data"
#define OBS_PLUGIN_PATH "plugins"
#define OBS_INSTALL_PLUGIN_PATH "plugins"
