#pragma once

#include "stdafx.h"

class JTextResource {
public:
	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }
	
protected:
	std::string m_name{};
};