#pragma once
#include "stdafx.h"
#include "JObject.h"


class JObjectRegistry {
public:
	using CreateFunc = std::function<std::shared_ptr<JObject>(std::vector<XMFLOAT4X4>&, std::vector<XMFLOAT4X4>&, UINT, const char*)>;

	static bool Register(const char* ObjectType, CreateFunc func);
	static std::shared_ptr<JObject> CreateObject(const char* objecttype, std::vector<XMFLOAT4X4>& matricesA, std::vector<XMFLOAT4X4>& matricesB,
		UINT index, const char* name);
private:
	static std::unordered_map<std::string, CreateFunc>& GetMap();
};



// Object관련 .cpp의 제일 아래에 사용
// 또한 그 .cpp는 #include "JObjectRegistry.h"를 필요로 한다.
// ex) JCustomObject.cpp
// ~~~~~~~~~~~~~~~~~~~~~~
// final line: REGISTER_OBJECT(JCustomObject)
#define REGISTER_OBJECT(ClassName)											\
	const bool ClassName##register =										\
		JObjectRegistry::Register(#ClassName,								\
			[](std::vector<XMFLOAT4X4>& a,									\
			   std::vector<XMFLOAT4X4>& b,									\
			   UINT index, const char* name)								\
				-> std::shared_ptr<JObject> {								\
				 return std::make_shared<ClassName>(a, b, index, name);		\
			   });