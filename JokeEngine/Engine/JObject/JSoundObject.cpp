#include "JSoundObject.h"
#include "JObjectRegistry.h"
#include "Engine/JContent/JSound.h"
#include "Engine/JSoundEngine/JSoundEngine.h"
#include "Engine/JSoundEngine/JSoundResourceXAudio2.h"

JSoundObject::JSoundObject(std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT uid, const char* name)
	: JObject(vWorld, vWorldTP, uid, name)
{
	m_SoundResource = std::make_unique<JSoundResourceXAudio2>();
}

void JSoundObject::Reset()
{

}

void JSoundObject::LoadObjectFromFile(nlohmann::json& object)
{

}

void JSoundObject::Play(bool begin)
{
	
}

void JSoundObject::Pause()
{

}

void JSoundObject::Stop()
{

}

void JSoundObject::SetVolume()
{

}

void JSoundObject::SetSubmixGroup(std::string& submix)
{

}

void JSoundObject::SetMultiSource(UINT num)
{

}

void JSoundObject::SetSound(JContent* sound)
{
	if (auto p = dynamic_cast<JSound*>(sound)) {
		m_Sound = p;
		m_SoundResource->SetSound(p);
	}
}

#if DEV_MODE
void JSoundObject::SaveObjectToFile(nlohmann::json& json, std::unordered_map<UINT, UINT>& IDtable)
{
	JObject::SaveObjectToFile(json, IDtable);

}

bool JSoundObject::RenderInspector(UINT currentFrameIndex)
{
	bool ret = JObject::RenderInspector(currentFrameIndex);
	return ret;
}

#endif

REGISTER_OBJECT(JSoundObject);