SET(CMAKE_SYSTEM_NAME Linux)
#指定C交叉编译器,必须配置
#或交叉编译器使用绝对地址
SET(CMAKE_C_COMPILER "arm-none-linux-gnueabi-gcc")
#指定C++交叉编译器
SET(CMAKE_CXX_COMPILER "arm-none-linux-gnueabi-g++")
SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/uhttpd_out")
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../json-c/json_out/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../json-c/json_out/lib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libubox/libubox_out/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../libubox/libubox_out/lib)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../ubus/ubus_out/include)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../ubus/ubus_out/lib)


cmake_minimum_required(VERSION 2.6)

PROJECT(uhttpd C)

INCLUDE (CheckFunctionExists)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
ADD_DEFINITIONS(-D_FILE_OFFSET_BITS=64 -Os -Wall -Werror -Wmissing-declarations --std=gnu99 -g3)

OPTION(TLS_SUPPORT "TLS support" ON)
OPTION(LUA_SUPPORT "Lua support" ON)
OPTION(UBUS_SUPPORT "ubus support" ON)

IF(APPLE)
  INCLUDE_DIRECTORIES(/opt/local/include)
  LINK_DIRECTORIES(/opt/local/lib)
ENDIF()

FIND_LIBRARY(LIBS crypt)
IF(LIBS STREQUAL "LIBS-NOTFOUND")
	SET(LIBS "")
ENDIF()

FIND_PATH(ubox_include_dir libubox/usock.h)
INCLUDE_DIRECTORIES(libubox/usock.h)

SET(SOURCES main.c listen.c client.c utils.c file.c auth.c cgi.c relay.c proc.c plugin.c handler.c)
IF(TLS_SUPPORT)
	SET(SOURCES ${SOURCES} tls.c)
	ADD_DEFINITIONS(-DHAVE_TLS)
ENDIF()

CHECK_FUNCTION_EXISTS(getspnam HAVE_SHADOW)
IF(HAVE_SHADOW)
    ADD_DEFINITIONS(-DHAVE_SHADOW)
ENDIF()

ADD_EXECUTABLE(uhttpd ${SOURCES})
FIND_LIBRARY(libjson NAMES json-c)
TARGET_LINK_LIBRARIES(uhttpd ubox dl json_script blobmsg_json json-c ${LIBS})

SET(PLUGINS "")
IF(LUA_SUPPORT)
	FIND_PROGRAM(PKG_CONFIG pkg-config)

	IF(NOT LUA_CFLAGS AND PKG_CONFIG)
		EXECUTE_PROCESS(
			COMMAND pkg-config --silence-errors --cflags lua5.1
			OUTPUT_VARIABLE LUA_CFLAGS
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	ENDIF()

	IF(NOT LUA_LIBS AND PKG_CONFIG)
		EXECUTE_PROCESS(
			COMMAND pkg-config --silence-errors --libs lua5.1
			OUTPUT_VARIABLE LUA_LIBS
			OUTPUT_STRIP_TRAILING_WHITESPACE
		)
	ENDIF()

	IF(NOT LUA_LIBS)
		SET(LUA_LIBS "lua")
	ENDIF()

	SET(PLUGINS ${PLUGINS} uhttpd_lua)
	ADD_DEFINITIONS(-DHAVE_LUA ${LUA_CFLAGS})
	ADD_LIBRARY(uhttpd_lua MODULE lua.c)
	TARGET_LINK_LIBRARIES(uhttpd_lua ${LUA_LIBS} m dl)
ENDIF()

IF(UBUS_SUPPORT)
	SET(PLUGINS ${PLUGINS} uhttpd_ubus)
	ADD_DEFINITIONS(-DHAVE_UBUS)
	ADD_LIBRARY(uhttpd_ubus MODULE ubus.c)
	TARGET_LINK_LIBRARIES(uhttpd_ubus ubus ubox blobmsg_json json-c)
ENDIF()

IF(PLUGINS)
	SET_TARGET_PROPERTIES(${PLUGINS} PROPERTIES
		PREFIX ""
	)
ENDIF()

INSTALL(TARGETS uhttpd ${PLUGINS}
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
)
