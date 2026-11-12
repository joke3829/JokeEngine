#pragma once

#include "Engine/JCommon.h"

// ======================================================================
// name: m_FullFilePath.filename()
// Content는 기본적으로 Resource 폴더 내에서 시작해야한다
// filepath가 상대경로 기준 + 시작이 무조건 Resource이기에 
// ======================================================================
class JContent : public JCommon {
public:
	// 무조건 파일 경로 넣기
	JContent(const char* name = nullptr);
	JContent(std::filesystem::path filepath);

	// 반드시 처음에 호출
	void Initialize();

	// 11: slot, 12: root parameter, 12는 parameter만 채우면 된다.
	virtual void SetGPUBuffer(UINT currentFrameIndex, UINT parameter, JShaderStage stage = JS_NONE) {}

	const std::filesystem::path& GetFullFilePath() { return m_FullFilePath; }

	virtual void Update(float elapsedTime) {}
	virtual void UpdateBuffer(UINT currentFrameIndex) {}

	virtual void Render(UINT currentFrameIndex) {}
#if DEV_MODE
public:
	virtual void RenderGUI(ImVec2 startPos, void* imageref = nullptr);
	void RenderGeneralInspect();
	virtual void RenderDetailPanel() { RenderGeneralInspect(); }
protected:
	bool m_ShowDetailGui{};
#endif

protected:
	// 파일이 없다면 새 content 파일 (json or yaml을 만들어라)
	virtual void CreateContentFile() {}	
	// 있다면 그걸 Load하라
	virtual void LoadFromFile() {}
	std::filesystem::path m_FullFilePath{};
};