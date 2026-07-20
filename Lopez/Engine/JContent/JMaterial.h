#include "JContent.h"

class JMaterial : public JContent {
public:
	JMaterial(const char* name = nullptr);
};



enum PBRMaterial_Tex {
	PBR_ALBEDO,
	PBR_ROUGHNESS
};

