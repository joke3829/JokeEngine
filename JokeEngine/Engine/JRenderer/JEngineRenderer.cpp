#include "JEngineRenderer.h"



JEngineRenderer::JEngineRenderer(UINT width, UINT height, const char* name)
	: JCommon(name)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::ResizeTarget(UINT width, UINT height)
{
	CreateRTV_DSV(width, height);
	m_TargetSize.x = width; m_TargetSize.y = height;
}

void JEngineRenderer::RenderFrame()
{
	if (not m_Scene)
		return;
	if (m_Scene->GetDirty()) {
		// 큐를 꺼내서 찾아 해메
		auto& q = m_Scene->GetCDQueue();
		while (not q.empty()) {
			auto& info = q.front();
			if (m_PSOMap.contains(info.shadername)) {
				if (info.object) {	// 추가
					m_PSOMap[info.shadername]->AddObject(info.uid, info.object);	// 방어 없음
				}
				else {		// 제거
					m_PSOMap[info.shadername]->DeleteObject(info.uid);
				}
			}
			q.pop();
		}
		m_Scene->SetDirty(false);
	}
}

void JEngineRenderer::AdvanceBufferIndex()
{
	m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % kNumRenderTarget;
}