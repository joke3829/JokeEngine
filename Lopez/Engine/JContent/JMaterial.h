#include "JContent.h"

class JMaterial : public JContent {
public:
	JMaterial(const char* name = nullptr);

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}
protected:
};



enum PBRMaterial_Tex {
	PBR_ALBEDO,
	PBR_ROUGHNESS
};

