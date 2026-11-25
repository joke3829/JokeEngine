#pragma once
#include "JObject.h"

class JSoundResource;
class JContent;
class JSound;


class JSoundObject : public JObject {
public:
	JSoundObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name = nullptr);

	virtual void Reset();	// 전체 멈춤, + 초기 상태
	virtual void LoadObjectFromFile(nlohmann::json& object);


	// 재생, 끊고 처음부터 재생
	void Play(bool begin = false);
	// 일시 정지
	void Pause();
	// 정지(가장 최근에 재생하것만 멈춤)
	void Stop(bool AllStop = false);
	// 볼륨 조절
	void SetVolume();
	// submix 그룹 선택
	void SetSubmixGroup(std::string& submix);
	// 동시 재생 가능 개수 선택(1~128)
	void SetMultiSource(UINT num);


	void SetSound(JContent* sound);
#if DEV_MODE
public:
	virtual void SaveObjectToFile(nlohmann::json& json, std::unordered_map<UINT, UINT>& IDtable);
	virtual bool RenderInspector(UINT currentFrameIndex = 0);
#endif
protected:
	UINT							m_NumSourceVoice{ 1 };

	JSound* m_Sound{};
	std::unique_ptr<JSoundResource> m_SoundResource{};
};