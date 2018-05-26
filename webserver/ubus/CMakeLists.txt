SET(CMAKE_SYSTEM_NAME Linux)
#指定C交叉编译器,必须配置
#或交叉编译器使用绝对地址
SET(CMAKE_C_COMPILER "arm-none-linux-gnueabi-gcc")
#指定C++交叉编译器
SET(CMAKE_CXX_COMPILER "arm-none-linux-gnueabi-g++")
SET(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/ubus_out")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../json-c/json_out/include/json-c)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../json-c/json_out/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libubox/libubox_out/include)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../libubox/libubox_out/lib)


cmake_minimum_required(VERSION 2.6)

PROJECT(ubus C)
ADD_DEFINITIONS(-Os -Wall -Werror --std=gnu99 -g3 -Wmissing-declarations)

OPTION(BUILD_LUA "build Lua plugin" ON)
OPTION(BUILD_EXAMPLES "build examples" ON)

SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")
SET(UBUS_UNIX_SOCKET "/var/run/ubus.sock")
SET(UBUS_MAX_MSGLEN 1048576)

ADD_DEFINITIONS( -DUBUS_UNIX_SOCKET="${UBUS_UNIX_SOCKET}")
ADD_DEFINITIONS( -DUBUS_MAX_MSGLEN=${UBUS_MAX_MSGLEN})

IF(BUILD_STATIC)
  FIND_LIBRARY(ubox_library NAMES ubox.a)
  FIND_LIBRARY(blob_library NAMES blobmsg_json.a)
ELSE(BUILD_STATIC)
  FIND_LIBRARY(ubox_library NAMES ubox)
  FIND_LIBRARY(blob_library NAMES blobmsg_json)
ENDIF(BUILD_STATIC)

FIND_PATH(ubox_include_dir libubox/usock.h)
INCLUDE_DIRECTORIES(libubox/usock.h)

ADD_LIBRARY(ubus SHARED libubus.c libubus-io.c libubus-obj.c libubus-sub.c libubus-req.c libubus-acl.c)
TARGET_LINK_LIBRARIES(ubus ubox)

find_library(json NAMES json-c)

ADD_EXECUTABLE(ubusd ubusd.c ubusd_id.c ubusd_obj.c ubusd_proto.c ubusd_event.c ubusd_acl.c ubusd_monitor.c)
TARGET_LINK_LIBRARIES(ubusd ubox blobmsg_json json-c)

ADD_EXECUTABLE(cli cli.c)
SET_TARGET_PROPERTIES(cli PROPERTIES OUTPUT_NAME ubus)
TARGET_LINK_LIBRARIES(cli ubus ubox blobmsg_json json-c)

ADD_SUBDIRECTORY(lua)
ADD_SUBDIRECTORY(examples)

INSTALL(TARGETS ubus cli
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
INSTALL(TARGETS ubusd
	RUNTIME DESTINATION sbin
)

INSTALL(FILES ubusmsg.h ubus_common.h libubus.h DESTINATION include)
