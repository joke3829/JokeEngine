#pragma once
#include "stdafx.h"

class JContent;

class JSoundResource {
public:
	virtual void SetSound(JContent* sound) {}
	virtual void SetVolume() {}
	virtual void Play(bool begin = false) {}
	virtual void Pause() {}
	virtual void Stop() {}

	virtual void SetSubMixGroup(std::string& submixname) {}
	virtual void SetMultiSource(UINT num) {}
};