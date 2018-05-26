# The set of languages for which implicit dependencies are needed:
SET(CMAKE_DEPENDS_LANGUAGES
  "C"
  )
# The set of files for implicit dependencies of each language:
SET(CMAKE_DEPENDS_CHECK_C
  "/home/gunix/workspace/src/webserver/ubus/examples/count.c" "/home/gunix/workspace/src/webserver/ubus/examples/CMakeFiles/server.dir/count.c.o"
  "/home/gunix/workspace/src/webserver/ubus/examples/server.c" "/home/gunix/workspace/src/webserver/ubus/examples/CMakeFiles/server.dir/server.c.o"
  )
SET(CMAKE_C_COMPILER_ID "GNU")

# Preprocessor definitions for this target.
SET(CMAKE_TARGET_DEFINITIONS
  "UBUS_MAX_MSGLEN=1048576"
  "UBUS_UNIX_SOCKET=\"/var/run/ubus.sock\""
  )

# Targets to which this target links.
SET(CMAKE_TARGET_LINKED_INFO_FILES
  "/home/gunix/workspace/src/webserver/ubus/CMakeFiles/ubus.dir/DependInfo.cmake"
  )

# The include file search paths:
SET(CMAKE_C_TARGET_INCLUDE_PATH
  "../json-c/json_out/include/json-c"
  "../libubox/libubox_out/include"
  "libubox/usock.h"
  "examples/.."
  )
SET(CMAKE_CXX_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_Fortran_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
SET(CMAKE_ASM_TARGET_INCLUDE_PATH ${CMAKE_C_TARGET_INCLUDE_PATH})
