#pragma once

#include "stdafx.h"

// name은 사실상 filepath

class JContent {
public:
	JContent(const char* name = nullptr);

	// 11: slot, 12: root parameter, 12는 parameter만 채우면 된다.
	virtual void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}

	virtual void Render() {}
	virtual void Render(void* command) {}

	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }
protected:
	// filepath로 처리하는거 안될 수 있다.(수정 필요)
	virtual void LoadFromFile(const char* name) {}
protected:
	std::string				m_name{};
};