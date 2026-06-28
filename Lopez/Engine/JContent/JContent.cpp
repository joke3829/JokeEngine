#include "JContent.h"

size_t g_numNoNameContent{};

JContent::JContent(const char* name)
{
	if (!name) m_name = "Content" + std::to_string(g_numNoNameContent++);
	else m_name = name;
}