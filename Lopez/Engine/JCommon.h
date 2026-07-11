#pragma once 

#include "stdafx.h"


class JCommon {
public:
	JCommon() = default;
	JCommon(const char* name) { m_name = name; }
	virtual ~JCommon() = default;


	void SetName(const char* name) { m_name = name; }
	const std::string& GetName() { return m_name; }
protected:
	std::string				m_name{};
};