# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_C
  "/home/gunix/workspace/src/webserver/ubus/ubusd.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_acl.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_acl.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_event.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_event.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_id.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_id.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_monitor.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_monitor.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_obj.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_obj.c.o"
  "/home/gunix/workspace/src/webserver/ubus/ubusd_proto.c" "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubusd.dir/ubusd_proto.c.o"
  )
SET(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "UBUS_MAX_MSGLEN=1048576"
  "UBUS_UNIX_SOCKET=\"/var/run/ubus.sock\""
  )

# Targets to which this target links.
SET(CMAKE_TARGET_LINKED_INFO_FILES
  )

# The include file search paths:
SET(CMAKE_C_TARGET_INCLUDE_PATH
  "../json-c/json_out/include/json-c"
  "../libubox/libubox_out/include"
  "libubox/usock.h"
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
