#pragma once

#include "stdafx.h"


// 옵션 Factor, 다음을 상속 해 옵션을 추가한다.
struct JEngineConfigFactor {
	virtual void InitFactorsByFile(nlohmann::json json);

	unsigned short	DirectX_Version{ 11 };

	bool			WindowsFullscreenState{ false };
	UINT			WindowsWidth{ 1280 };
	UINT			WindowsHeight{ 720 };

	float			LimitFPS{ 120.f };		// 0.f일 땐 Unlimit Frame
};

// ==============================================================

// 옵션 Factor를 관리하는 class, 다음을 상속해 게임에 맞는 옵션을 만들도록 한다.
// 명시적으로 자신의 프로젝트에 맞게 생성해 사용하도록 설계하였다.
class JokeEngineGlobalConfigBase {
public:
	virtual void OptionSaveAndFileGenerate() {};
protected:
	JokeEngineGlobalConfigBase() {};
	virtual ~JokeEngineGlobalConfigBase() {};
};


// example
// 되도록이면 싱글톤 구조를 추천
class JokeEngineGlobalConfigExample : public JokeEngineGlobalConfigBase {
public:
	static JokeEngineGlobalConfigExample* GetInstance();
	JEngineConfigFactor* GetConfigFactor();

	void OptionSaveAndFileGenerate();
private:
	JokeEngineGlobalConfigExample();
	~JokeEngineGlobalConfigExample();

	static JokeEngineGlobalConfigExample* m_Instance;

	JEngineConfigFactor* m_ConfigFactor;
};