cmake_minimum_required(VERSION 3.24)


project(Lopez)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Available build configurations" FORCE)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose Debug or Release" FORCE)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(RESOURCE_FILES
  Lopez.rc
  Resource.h
)

set(COMMON_HEADER
  stdafx.h
  targetver.h
)

set(COMMON_SOURCE
  Lopez.cpp
)

add_executable(${PROJECT_NAME} WIN32 ${RESOURCE_FILES} ${COMMON_HEADER} ${COMMON_SOURCE})