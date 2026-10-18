cmake_minimum_required(VERSION 3.20)

# initialize pico-sdk from GIT
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_FETCH_FROM_GIT on)
set(PICOTOOL_FORCE_FETCH_FROM_GIT on)

# Set board type
# note: this must happen before the pico_sdk_import include
set(PICO_BOARD waveshare_rp2040_zero)
set(CMAKE_BUILD_TYPE Release)

# pico_sdk_import.cmake is a single file copied from this SDK
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(xbox360slimRF)

#set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# initialize the Raspberry Pi Pico SDK
pico_sdk_init()
add_compile_options(
	-Wall
	-Wextra
	-pedantic
	$<$<COMPILE_LANGUAGE:C>:-std=gnu11>
	$<$<COMPILE_LANGUAGE:CXX>:-std=gnu++14>
)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Right button module
add_executable(xbox360slimRF
  main.cpp
)

#target_compile_definitions(xbox360slimRF
#
#)

target_link_libraries(xbox360slimRF
  pico_stdlib
)

pico_add_extra_outputs(xbox360slimRF)
