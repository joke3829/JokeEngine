#include "JContent.h"

class JMaterialSimple : public JContent {
public:
	JMaterialSimple(const char* name = nullptr);

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}
protected:
	//std::unordered_map<std::string, std::shared_ptr<JContent>>& m_ContentLookupTable;		// 여기서 텍스쳐 이름으로 찾기
};