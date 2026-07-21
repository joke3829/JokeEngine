// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

// 헤더 선언부 ==============================================================================

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cassert>

#include <string>
#include <algorithm>
#include <vector>
#include <memory>
#include <fstream>
#include <unordered_map>

#include <d3d11_4.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <wrl.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <comdef.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;


// 외부 라이브러리
#if defined(_DEBUG) || defined(DEBUG)
#include "Package/spdlog/spdlog.h"
#endif
#include "Package/nlohmann/json.hpp"



// 공용 함수 선언부===============================================================================

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr)) {
#if defined(_DEBUG) || defined(DEBUG)
		_com_error err(hr);
		LPCSTR errMsg = err.ErrorMessage();
		spdlog::critical("[FATAL ERROR] DirectX API Failed, HRESULT: 0x{0:X} | Message: {1}",
			static_cast<unsigned int>(hr), errMsg);
#endif
		assert(0);
	}
}


// size를 multiple의 배수로 변환한 값을 반환
// multiple이 2의 제곱수일 때 정상 작동
inline UINT Align(UINT size, UINT multiple)
{
	return (size + (multiple - 1)) & ~(multiple - 1);
}


#if defined(_DEBUG) || defined(DEBUG)
inline void ShowInsertedFailed(const std::string& name)
{
	spdlog::warn("{0} (이)가 이미 ContentManager에 있어 추가에 실패했습니다.", name.c_str());
}
#endif

// DX 상수, 열거자 =======================================================================

enum JShaderStage : unsigned char {
	JS_NONE,
	JS_VS,
	JS_PS,
	JS_GS,
	JS_HS,
	JS_DS,
	JS_CS
};

// 렌더 타겟 수 - DX12의 경우 fence를 2개를 이용해 렌더링 시간 손해 없---
constexpr UINT									g_NumRenderTarget = 2;