cmake_minimum_required(VERSION 3.24)


project(Lopez)

if(MSVC)
	add_compile_options(/utf-8)
endif()

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
  stdafx.cpp
)

# Engine 관련 Set ==================================================

set(ENGINE
  Engine/JokeEngine.h
  Engine/JokeEngine.cpp
  Engine/JokeEngineConfig.h
  Engine/JokeEngineConfig.cpp
  Engine/JCommon.h
  Engine/JCommon.cpp
  Engine/JMeshConstant.h
  Engine/JMeshConstant.cpp
  Engine/Timer.h
  Engine/Timer.cpp
)

set(ENGINE_OBJECT
  Engine/JObject/JObject.h
  Engine/JObject/JObject.cpp
  Engine/JObject/JCameraObject.h
  Engine/JObject/JCameraObject.cpp
  Engine/JObject/JStaticMeshObject.h
  Engine/JObject/JStaticMeshObject.cpp
)

set(ENGINE_CONTENT
  Engine/JContent/JContent.h
  Engine/JContent/JContent.cpp
  Engine/JContent/JMaterial.h
  Engine/JContent/JMaterial.cpp
  Engine/JContent/JStaticMesh.h
  Engine/JContent/JStaticMesh.cpp
  Engine/JContent/JContentManager.h
  Engine/JContent/JContentManager.cpp
  Engine/JContent/JTexture.h
  Engine/JContent/JTexture.cpp
)

set(ENGINE_RENDERER
  Engine/JRenderer/JEngineRenderer.h
  Engine/JRenderer/JEngineRenderer.cpp
)

set(ENGINE_SCENE
  Engine/JScene/JEngineScene.h
  Engine/JScene/JEngineScene.cpp
)

set(ENGINE_SHADER
  Engine/JShader/JEngineShader.h
  Engine/JShader/JEngineShader.cpp
)

set(ENGINE_INTEGRATE
  ${ENGINE}
  ${ENGINE_OBJECT}
  ${ENGINE_CONTENT}
  ${ENGINE_RENDERER}
  ${ENGINE_SCENE}
  ${ENGINE_SHADER}
)

# D3D11 관련 Set =================================================

set(D3D11
  Engine/D3D11/D3D11GlobalFactor.h
  Engine/D3D11/D3D11GlobalFactor.cpp
  Engine/D3D11/D3D11MeshConstant.h
  Engine/D3D11/D3D11MeshConstant.cpp
)

set(D3D11_CONTENT
  Engine/D3D11/Content/D3D11Material.h
  Engine/D3D11/Content/D3D11Material.cpp
  Engine/D3D11/Content/D3D11StaticMesh.h
  Engine/D3D11/Content/D3D11StaticMesh.cpp
  Engine/D3D11/Content/D3D11ContentManager.h
  Engine/D3D11/Content/D3D11ContentManager.cpp
  Engine/D3D11/Content/D3D11Texture.h
  Engine/D3D11/Content/D3D11Texture.cpp
)


set(D3D11_RENDERER
  Engine/D3D11/Renderer/D3D11Renderer.h
  Engine/D3D11/Renderer/D3D11Renderer.cpp
)

set(D3D11_SCENE
  Engine/D3D11/Scene/D3D11Scene.h
  Engine/D3D11/Scene/D3D11Scene.cpp
)

set(D3D11_SHADER
  Engine/D3D11/Shader/D3D11Shader.h
  Engine/D3D11/Shader/D3D11Shader.cpp
)

set(D3D11_INTEGRATE
  ${D3D11}
  ${D3D11_CONTENT}
  ${D3D11_RENDERER}
  ${D3D11_SCENE}
  ${D3D11_SHADER}
)



set(SHADER_FILES
  Shaders/Default/Common.hlsl
  Shaders/Default/DefaultShader.hlsl
)

set(TEXTURE_LOADER
  Package/DirectXTex/d3dx12.h
  Package/DirectXTex/DDSTextureLoader11.h
  Package/DirectXTex/DDSTextureLoader11.cpp
  Package/DirectXTex/DDSTextureLoader12.h
  Package/DirectXTex/DDSTextureLoader12.cpp
  Package/DirectXTex/WICTextureLoader11.h
  Package/DirectXTex/WICTextureLoader11.cpp
  Package/DirectXTex/WICTextureLoader12.h
  Package/DirectXTex/WICTextureLoader12.cpp
)


add_executable(${PROJECT_NAME} 
   WIN32 ${RESOURCE_FILES} 
   ${COMMON_HEADER} 
   ${COMMON_SOURCE} 
   ${ENGINE_INTEGRATE}
   ${D3D11_INTEGRATE}
   ${SHADER_FILES}
   ${TEXTURE_LOADER}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Package
)


source_group("Engine" FILES ${ENGINE})
source_group("Engine\\JObject" FILES ${ENGINE_OBJECT})
source_group("Engine\\JContent" FILES ${ENGINE_CONTENT})
source_group("Engine\\JRenderer" FILES ${ENGINE_RENDERER})
source_group("Engine\\JScene" FILES ${ENGINE_SCENE})
source_group("Engine\\JShader" FILES ${ENGINE_SHADER})

source_group("Engine\\D3D11" FILES ${D3D11})
source_group("Engine\\D3D11\\Content" FILES ${D3D11_CONTENT})
source_group("Engine\\D3D11\\Renderer" FILES ${D3D11_RENDERER})
source_group("Engine\\D3D11\\Scene" FILES ${D3D11_SCENE})
source_group("Engine\\D3D11\\Shader" FILES ${D3D11_SHADER})


# TextureLoader 추가
source_group("TextureLoader" FILES ${TEXTURE_LOADER})

# Shader File 솔루션에 추가 + 빌드에 참여 안함
source_group("Shaders" FILES ${SHADER_FILES})
set_source_files_properties(${SHADER_FILES} PROPERTIES VS_TOOL_OVERRIDE "None")

# 파일 경로 솔루션 f5시 CMakeLists.txt위치를 보게
set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")