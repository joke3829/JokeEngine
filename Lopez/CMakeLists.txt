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

set(D3D11
  Engine/D3D11/D3D11GlobalFactor.h
  Engine/D3D11/D3D11GlobalFactor.cpp
)

set(ENGINE
  Engine/JokeEngine.h
  Engine/JokeEngine.cpp
)

add_executable(${PROJECT_NAME} 
   WIN32 ${RESOURCE_FILES} 
   ${COMMON_HEADER} 
   ${COMMON_SOURCE} 
   ${ENGINE}
   ${D3D11}
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

source_group("Engine" FILES ${ENGINE})
source_group("Engine\\D3D11" FILES ${D3D11})