#pragma once

#include "JContent.h"
#include "JMaterial.h"

// 이놈은 반드시 텍스쳐를 가진 마테리얼 set 필요
// Material이 Sprite이미지



// KeyFrame이나 마찬가지
struct JSAnimData {
	UINT materialIndex;		// 마테리얼 중 어떤게 내꺼냐
	float keyframeTime;		// 키프레임 시간
	float perWidth;			// 스프라이트 하나당 width
	float perHeight;		// 하나당 height
	float ImageWidth;		// 이미지 전체 width
	float ImageHeight;		// 전체 height
	float row;				// 그 중 행
	float col;				// 열
};

class JSpriteAnimationSet : public JContent {
public:
	virtual void AddMaterial(std::shared_ptr<JContent>& material) {}



	// 이름과 시간 주면 JSAnimData 받기
	const JSAnimData& GetKeyFrameData(const char* animName, float& time); // time은 player가 관리 하되 현재 애니메이션의 시간을 초과하면 time을 초과한 시간을 제거하고 돌려줌

	void LoadAnimationFactorFromYaml(const char* filepath);
protected:
	std::unordered_map < std::string, std::vector<JSAnimData>> m_KeyFrame{};	// 반드시 0번째라도 채워두자
};