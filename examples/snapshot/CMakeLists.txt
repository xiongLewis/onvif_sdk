cmake_minimum_required(VERSION 2.8)
project(discovery_demo)

add_definitions("-Wall -g -DSOAP_DEBUG -DWITH_DOM -DWITH_OPENSSL") # --verbose
#add_compile_options("-std=gnu++11")
set(CMAKE_SYSTEM_NAME Linux)
#set(CMAKE_C_COMPILER "aarch64-himix100-linux-gcc")
#set(CMAKE_CXX_COMPILER "aarch64-himix100-linux-g++")
#set(CMAKE_SYSTEM_PROCESSOR "arm")


string(REGEX REPLACE ".*/\(.*\)" "\\1" CURDIR ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include
					${CMAKE_CURRENT_SOURCE_DIR}/../../soap_framework/include)

SET(SRC_LIST snapshot.c
			../../soap_framework/src/soapC.c
			../../soap_framework/src/soapClient.c
			../../soap_framework/src/stdsoap2.c
			../../soap_framework/src/struct_timeval.c
			../../soap_framework/src/wsaapi.c
			../../soap_framework/src/wsseapi.c
			../../soap_framework/src/smdevp.c
			../../soap_framework/src/mecevp.c
			../../soap_framework/src/dom.c
			../../utils/print_log.c
			../../utils/onvif_debug.c
			../../src/onvif_comm.c
			../../src/onvif_discovery.c
			../../src/onvif_service.c)	 

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/x86_64/)

ADD_EXECUTABLE(snapshot_demo ${SRC_LIST})

target_link_libraries(snapshot_demo
						pthread
						ssl
						crypto
						dl
						m
						rt
						z
						)
