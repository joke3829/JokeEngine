#pragma once

#include "JContent.h"
#include "JMaterial.h"


// Material은 Object가 가질 것



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

// keyframe에 대한 정보만 가진다.
class JSpriteAnimationSet : public JContent {
public:
	JSpriteAnimationSet(const char* name = nullptr);

	const JSAnimData& GetKeyFrameData(std::string& animName, float& time); // time은 player가 관리 하되 현재 애니메이션의 시간을 초과하면 time을 초과한 시간을 제거하고 돌려줌

	void LoadAnimationFactorFromYaml(const char* filepath);
protected:
	std::unordered_map<std::string, std::vector<JSAnimData>>	m_KeyFrame{};	// 반드시 0번째라도 채워두자
	std::unordered_map<std::string, float>						m_PlayTime{};	// 애니메이션 별 엔드타임
};