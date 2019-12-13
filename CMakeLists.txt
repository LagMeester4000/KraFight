cmake_minimum_required(VERSION 3.13.0)
cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0077 NEW)
project(KraFight)

# User options

set(APP_NAME LagMeester4000\ KraFight)
set(APP_PACKAGE KraFight)
set(IS_GAME true)
set(APP_VERSION 1.0) 

# Default options

add_definitions(-D__PROJECT_NAME__="${APP_NAME}")

set(HOME_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_SUPPRESS_REGENERATION true)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Enable multi processor compilation. Use C++ standard and enable all warnings (except nameless structs)
if(MSVC)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD /MP /W4 /permissive- /wd4201 /EHsc")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd /MP /W4 /permissive- /wd4201 /EHsc")
endif()

# No console option

option(no_console "no_console" OFF)

if(no_console)
	add_definitions(-D__NO_CONSOLE__)
	message ("-- Console - disabled")
else()
	message ("-- Console - enabled")
endif()


if(WIN32)

	set(Vulkan ON)
	add_definitions(-D__DESKTOP__)
	add_definitions(-D__WINDOWS__)
	set(Windows ON)
	set(Desktop ON)
	
	option(Raytracing "Raytracing" ON)
		
	link_libraries(Xinput9_1_0.lib)

else()
endif()


if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-D__DEBUG__)
	message("-- Debug - enabled")
endif()

# dependencies
add_subdirectory(deps)
	
# libs projects
add_subdirectory(KraFight)
set_property(TARGET KraFight PROPERTY FOLDER KraFight)
add_subdirectory(KraNet)
set_property(TARGET KraNet PROPERTY FOLDER KraNet)

# app projects
add_subdirectory(KraRenderer)
set_property(TARGET KraRenderer PROPERTY FOLDER application)

if(Windows)
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT KraRenderer)
endif()