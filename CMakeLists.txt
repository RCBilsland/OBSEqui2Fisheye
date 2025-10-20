cmake_minimum_required(VERSION 3.20)

project(obs-equi2fisheye VERSION 0.1.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Try to find libobs via pkg-config first
find_package(PkgConfig)
set(HAVE_PKGCFG FALSE)
if(PkgConfig_FOUND)
	pkg_check_modules(LIBOBS libobs)
	if(LIBOBS_FOUND)
		set(HAVE_PKGCFG TRUE)
	endif()
endif()

include(GNUInstallDirs)

set(PLUGIN_NAME obs-equi2fisheye)

add_library(${PLUGIN_NAME} MODULE
	src/obs-equi2fisheye.c
)

if(HAVE_PKGCFG)
	target_include_directories(${PLUGIN_NAME}
		PRIVATE
		${LIBOBS_INCLUDE_DIRS}
	)
	link_directories(${LIBOBS_LIBRARY_DIRS})
	target_link_libraries(${PLUGIN_NAME}
		PRIVATE
		${LIBOBS_LIBRARIES}
	)
else()
	message(STATUS "pkg-config for libobs not found; using env-provided include/lib dirs")
	if(DEFINED ENV{OBS_INCLUDE_DIR})
		target_include_directories(${PLUGIN_NAME} PRIVATE $ENV{OBS_INCLUDE_DIR})
	endif()
	if(DEFINED ENV{OBS_GENERATED_INCLUDE_DIR})
		message(STATUS "Using generated OBS headers from $ENV{OBS_GENERATED_INCLUDE_DIR}")
		target_include_directories(${PLUGIN_NAME} PRIVATE $ENV{OBS_GENERATED_INCLUDE_DIR})
	endif()
	if(DEFINED ENV{OBS_GENERATED_LIBOBS_INCLUDE_DIR})
		message(STATUS "Using generated libobs headers from $ENV{OBS_GENERATED_LIBOBS_INCLUDE_DIR}")
		target_include_directories(${PLUGIN_NAME} PRIVATE $ENV{OBS_GENERATED_LIBOBS_INCLUDE_DIR})
	endif()
	if(DEFINED ENV{SIMDE_INCLUDE_DIR})
		message(STATUS "Using SIMDE headers from $ENV{SIMDE_INCLUDE_DIR}")
		target_include_directories(${PLUGIN_NAME} PRIVATE $ENV{SIMDE_INCLUDE_DIR})
	endif()
	if(DEFINED ENV{OBS_LIB_DIR})
		link_directories($ENV{OBS_LIB_DIR})
		# Try to find the OBS library in the app bundle
		find_library(OBS_LIBRARY
			NAMES obs libobs
			PATHS $ENV{OBS_LIB_DIR}
			NO_DEFAULT_PATH
		)
		if(OBS_LIBRARY)
			target_link_libraries(${PLUGIN_NAME} PRIVATE ${OBS_LIBRARY})
		else()
			# Fallback: try to link against obs by name
			target_link_libraries(${PLUGIN_NAME} PRIVATE obs)
		endif()
	else()
		# Fallback: try to link against obs by name
		target_link_libraries(${PLUGIN_NAME} PRIVATE obs)
	endif()
endif()

if(APPLE)
	# Allow undefined symbols to be resolved at runtime by OBS on macOS bundles
	target_link_options(${PLUGIN_NAME} PRIVATE -Wl,-undefined,dynamic_lookup)
	set_target_properties(${PLUGIN_NAME} PROPERTIES
		BUNDLE TRUE
		BUNDLE_EXTENSION plugin
		OUTPUT_NAME ${PLUGIN_NAME}
	)
endif()

# Install plugin binary and data
set(OBS_PLUGIN_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/obs-plugins")
set(OBS_DATA_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/obs")

install(TARGETS ${PLUGIN_NAME}
	BUNDLE DESTINATION ${OBS_PLUGIN_INSTALL_DIR}
	LIBRARY DESTINATION ${OBS_PLUGIN_INSTALL_DIR}
)

install(DIRECTORY data/ DESTINATION ${OBS_DATA_INSTALL_DIR}/plugins/${PLUGIN_NAME})
