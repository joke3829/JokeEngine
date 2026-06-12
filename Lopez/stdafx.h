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

#include <d3d11_4.h>
#include <dxgi1_6.h>

#include <wrl.h>
#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include <comdef.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;


// 디버그용 외부 라이브러리
#if defined(_DEBUG) || defined(DEBUG)
#include "Package/spdlog/spdlog.h"
#endif


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