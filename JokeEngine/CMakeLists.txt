cmake_minimum_required(VERSION 3.24)


project(Lopez)

if(MSVC)
	add_compile_options(/utf-8)
	add_compile_definitions(UNICODE _UNICODE)
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Develop" CACHE STRING "Available build configurations" FORCE)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose Debug or Release" FORCE)
endif()

add_compile_definitions($<$<CONFIG:Develop>:DEVELOP>)


# Develop 빌드 타입이 Release의 컴파일/링크 옵션을 그대로 상속받도록 설정
# (CMake는 기본적으로 사용자 정의 빌드 타입에 대해 최적화나 런타임 라이브러리 설정을 모르므로 명시해 주어야 합니다)
foreach(lang C CXX)
    # 컴파일 플래그 (예: /O2 최적화 등 Release 설정 복사)
    set(CMAKE_${lang}_FLAGS_DEVELOP "${CMAKE_${lang}_FLAGS_RELEASE}")
    
    # 전처리기 정의 (NDEBUG 등)
    set(CMAKE_${lang}_FLAGS_DEVELOP_INIT "${CMAKE_${lang}_FLAGS_RELEASE_INIT}")
endforeach()

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
  Engine/Timer.h
  Engine/Timer.cpp
  Engine/JInputThread.h
  Engine/JInputThread.cpp
)

set(ENGINE_GRESOURCE
  Engine/GResource/JMeshConstant.h
  Engine/GResource/JSpriteMeshConstant.h
  Engine/GResource/JSpriteMeshConstant.cpp
  Engine/GResource/JTextResource.h
  Engine/GResource/JTextResource.cpp
)

set(ENGINE_OBJECT
  Engine/JObject/JObject.h
  Engine/JObject/JObject.cpp
  Engine/JObject/JCameraObject.h
  Engine/JObject/JCameraObject.cpp
  Engine/JObject/JStaticMeshObject.h
  Engine/JObject/JStaticMeshObject.cpp
  Engine/JObject/JPlayerObject.h
  Engine/JObject/JPlayerObject.cpp
  Engine/JObject/JSpriteAnimationObject.h
  Engine/JObject/JSpriteAnimationObject.cpp
  Engine/JObject/JTextObject.h
  Engine/JObject/JTextObject.cpp
  Engine/JObject/JEditorPlayer.h
  Engine/JObject/JEditorPlayer.cpp
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
  Engine/JContent/JSpriteAnimationSet.h
  Engine/JContent/JSpriteAnimationSet.cpp
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
  ${ENGINE_GRESOURCE}
  ${ENGINE_OBJECT}
  ${ENGINE_CONTENT}
  ${ENGINE_RENDERER}
  ${ENGINE_SCENE}
  ${ENGINE_SHADER}
)

# D2D 관련 Set ===================================================

set(D2D
  Engine/D2D/JD2DTextEngine.h
  Engine/D2D/JD2DTextEngine.cpp
  Engine/D2D/DWriteTextResource.h
  Engine/D2D/DWriteTextResource.cpp
)

set(D2D_INTEGRATE
  ${D2D}
)

# D3D11 관련 Set =================================================

set(D3D11
  Engine/D3D11/D3D11GlobalFactor.h
  Engine/D3D11/D3D11GlobalFactor.cpp
)

set(D3D11_GRESOURCE
  Engine/D3D11/GResource/D3D11MeshConstant.h
  Engine/D3D11/GResource/D3D11MeshConstant.cpp
  Engine/D3D11/GResource/D3D11SpriteMeshConstant.h
  Engine/D3D11/GResource/D3D11SpriteMeshConstant.cpp
  Engine/D3D11/GResource/D3D11TextResource.h
  Engine/D3D11/GResource/D3D11TextResource.cpp
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
  ${D3D11_GRESOURCE}
  ${D3D11_CONTENT}
  ${D3D11_RENDERER}
  ${D3D11_SCENE}
  ${D3D11_SHADER}
)


# Shader 관련 Set ===================================================

set(SHADER_FILES
  Shaders/Default/Common.hlsl
  Shaders/Default/DefaultMaterialShader.hlsl
  Shaders/Default/UVTransformShader.hlsl
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

# 제작할 게임에 맞는 파일 추가 ===========================================================

# ========================================================================================


add_executable(${PROJECT_NAME} 
   WIN32 ${RESOURCE_FILES} 
   ${COMMON_HEADER} 
   ${COMMON_SOURCE} 
   ${ENGINE_INTEGRATE}
   ${D2D_INTEGRATE}
   ${D3D11_INTEGRATE}
   ${SHADER_FILES}
   ${TEXTURE_LOADER}
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Package
  ${CMAKE_CURRENT_SOURCE_DIR}/Package/imgui
  ${CMAKE_CURRENT_SOURCE_DIR}/Package/imgui/backends
)

# imgui =========================================

set(PKG_IMGUI
  Package/imgui/imconfig.h
  Package/imgui/imgui.cpp
  Package/imgui/imgui.h
  Package/imgui/imgui_demo.cpp
  Package/imgui/imgui_draw.cpp
  Package/imgui/imgui_internal.h
  Package/imgui/imgui_tables.cpp
  Package/imgui/imgui_widgets.cpp
  Package/imgui/imstb_rectpack.h
  Package/imgui/imstb_textedit.h
  Package/imgui/imstb_truetype.h
  Package/imgui/backends/imgui_impl_dx11.cpp
  Package/imgui/backends/imgui_impl_dx11.h
  Package/imgui/backends/imgui_impl_dx12.cpp
  Package/imgui/backends/imgui_impl_dx12.h
  Package/imgui/backends/imgui_impl_win32.cpp
  Package/imgui/backends/imgui_impl_win32.h
  Package/imgui/ImGuizmo.h
  Package/imgui/ImGuizmo.cpp
)

target_sources(${PROJECT_NAME} PRIVATE
  $<$<OR:$<CONFIG:Debug>,$<CONFIG:Develop>>:${PKG_IMGUI}>
)


source_group("Engine" FILES ${ENGINE})
source_group("Engine\\GResource" FILES ${ENGINE_GRESOURCE})
source_group("Engine\\JObject" FILES ${ENGINE_OBJECT})
source_group("Engine\\JContent" FILES ${ENGINE_CONTENT})
source_group("Engine\\JRenderer" FILES ${ENGINE_RENDERER})
source_group("Engine\\JScene" FILES ${ENGINE_SCENE})
source_group("Engine\\JShader" FILES ${ENGINE_SHADER})

source_group("Engine\\D2D" FILES ${D2D})

source_group("Engine\\D3D11" FILES ${D3D11})
source_group("Engine\\D3D11\\GResource" FILES ${D3D11_GRESOURCE})
source_group("Engine\\D3D11\\Content" FILES ${D3D11_CONTENT})
source_group("Engine\\D3D11\\Renderer" FILES ${D3D11_RENDERER})
source_group("Engine\\D3D11\\Scene" FILES ${D3D11_SCENE})
source_group("Engine\\D3D11\\Shader" FILES ${D3D11_SHADER})


# TextureLoader 추가
source_group("TextureLoader" FILES ${TEXTURE_LOADER})

# imgui source_group
source_group("imgui" FILES ${PKG_IMGUI})

# Shader File 솔루션에 추가 + 빌드에 참여 안함
source_group("Shaders" FILES ${SHADER_FILES})
set_source_files_properties(${SHADER_FILES} PROPERTIES VS_TOOL_OVERRIDE "None")

# 파일 경로 솔루션 f5시 CMakeLists.txt위치를 보게
set_target_properties(${PROJECT_NAME} PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")