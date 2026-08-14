#pragma once

#include "stdafx.h"


// 옵션 Factor, 다음을 상속 해 옵션을 추가한다.
struct JEngineConfigDefaultFactor {
	virtual void InitFactorsByFile(nlohmann::json json);

	unsigned short	DirectX_Version{ 11 };

	bool			WindowsFullscreenState{ false };
	UINT			WindowsWidth{ 1280 };
	UINT			WindowsHeight{ 720 };

	bool			VerticalSYNC{ false };
	float			LimitFPS{ 120.f };		// 0.f일 땐 Unlimit Frame
};

// ==============================================================

// 옵션 Factor를 관리하는 class, 다음을 상속해 게임에 맞는 옵션을 만들도록 한다.
// 명시적으로 자신의 프로젝트에 맞게 생성해 사용하도록 설계하였다.

// 옵션파일이 비정상 적용 되면 JEngine_Config.json을 삭제하고 다시 실행한다.

class JEngineGlobalConfigBase {
public:
	virtual void OptionSaveAndFileGenerate() {};
protected:
	JEngineGlobalConfigBase() {};
	virtual ~JEngineGlobalConfigBase() {};
};


// example
// 되도록이면 싱글톤 구조를 추천
class JEngineDefaultGlobalConfig : public JEngineGlobalConfigBase {
public:
	static JEngineDefaultGlobalConfig* GetInstance();
	JEngineConfigDefaultFactor* GetConfigFactor();

	void OptionSaveAndFileGenerate();
private:
	JEngineDefaultGlobalConfig();
	~JEngineDefaultGlobalConfig();

	static JEngineDefaultGlobalConfig* m_Instance;

	JEngineConfigDefaultFactor* m_ConfigFactor;
};