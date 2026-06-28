#include "JContent.h"

class JMaterialSimple : public JContent {
public:
	JMaterialSimple(const char* name = nullptr);

	virtual void SetDXBuffer(UINT parameter, JShaderStage stage = JS_NONE) {}
protected:
};



enum PBRMaterial_Tex {
	PBR_ALBEDO,
	PBR_ROUGHNESS
};