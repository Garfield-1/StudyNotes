# Install script for directory: /home/qiujian/only_test/ubus/json-c

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/qiujian/only_test/ubus/jsonC_build")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so.5.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so.5"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      file(RPATH_CHECK
           FILE "${file}"
           RPATH "")
    endif()
  endforeach()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES
    "/home/qiujian/only_test/ubus/jsonC_build/libjson-c.so.5.3.0"
    "/home/qiujian/only_test/ubus/jsonC_build/libjson-c.so.5"
    "/home/qiujian/only_test/ubus/jsonC_build/libjson-c.so"
    )
  foreach(file
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so.5.3.0"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so.5"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libjson-c.so"
      )
    if(EXISTS "${file}" AND
       NOT IS_SYMLINK "${file}")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/usr/bin/strip" "${file}")
      endif()
    endif()
  endforeach()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/qiujian/only_test/ubus/jsonC_build/libjson-c.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake"
         "/home/qiujian/only_test/ubus/jsonC_build/CMakeFiles/Export/lib/cmake/json-c/json-c-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c/json-c-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/qiujian/only_test/ubus/jsonC_build/CMakeFiles/Export/lib/cmake/json-c/json-c-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/qiujian/only_test/ubus/jsonC_build/CMakeFiles/Export/lib/cmake/json-c/json-c-targets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/json-c" TYPE FILE FILES "/home/qiujian/only_test/ubus/jsonC_build/json-c-config.cmake")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/qiujian/only_test/ubus/jsonC_build/json-c.pc")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_config.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/arraylist.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/debug.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_c_version.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_inttypes.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_object.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_object_iterator.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_tokener.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_types.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_util.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_visit.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/linkhash.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/printbuf.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_pointer.h;/home/qiujian/only_test/ubus/jsonC_build/include/json-c/json_patch.h")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "/home/qiujian/only_test/ubus/jsonC_build/include/json-c" TYPE FILE FILES
    "/home/qiujian/only_test/ubus/jsonC_build/json_config.h"
    "/home/qiujian/only_test/ubus/jsonC_build/json.h"
    "/home/qiujian/only_test/ubus/json-c/arraylist.h"
    "/home/qiujian/only_test/ubus/json-c/debug.h"
    "/home/qiujian/only_test/ubus/json-c/json_c_version.h"
    "/home/qiujian/only_test/ubus/json-c/json_inttypes.h"
    "/home/qiujian/only_test/ubus/json-c/json_object.h"
    "/home/qiujian/only_test/ubus/json-c/json_object_iterator.h"
    "/home/qiujian/only_test/ubus/json-c/json_tokener.h"
    "/home/qiujian/only_test/ubus/json-c/json_types.h"
    "/home/qiujian/only_test/ubus/json-c/json_util.h"
    "/home/qiujian/only_test/ubus/json-c/json_visit.h"
    "/home/qiujian/only_test/ubus/json-c/linkhash.h"
    "/home/qiujian/only_test/ubus/json-c/printbuf.h"
    "/home/qiujian/only_test/ubus/json-c/json_pointer.h"
    "/home/qiujian/only_test/ubus/json-c/json_patch.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/qiujian/only_test/ubus/jsonC_build/doc/cmake_install.cmake")
  include("/home/qiujian/only_test/ubus/jsonC_build/tests/cmake_install.cmake")
  include("/home/qiujian/only_test/ubus/jsonC_build/apps/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/qiujian/only_test/ubus/jsonC_build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
