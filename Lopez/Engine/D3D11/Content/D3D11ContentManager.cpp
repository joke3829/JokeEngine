#include "D3D11ContentManager.h"
#include "D3D11Material.h"
#include "D3D11StaticMesh.h"

void JContentManagerDX11::ReadyDefaultContent()
{
	// 1. Default Material 준비

	{
		std::shared_ptr<JMaterialDX11> material = std::make_shared<JMaterialDX11>("DefaultMaterial");
		
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(material->GetName(), material);
#if defined(_DEBUG) || defined(DEBUG)
		if(not inserted) ShowInsertedFailed(material->GetName());
#endif
	}

	// 2. Default StaticMesh 준비
	{
		std::shared_ptr<JStaticMeshDX11> mesh = std::make_shared<JStaticMeshDX11>(XMFLOAT3(0.f, 0.f, 0.f), XMFLOAT3(2.5f, 2.5f, 2.5f), "DefaultCube");
		auto [iter, inserted] = m_ContentLookupTable.try_emplace(mesh->GetName(), mesh);
#if defined(_DEBUG) || defined(DEBUG)
		if(not inserted) ShowInsertedFailed(mesh->GetName());
#endif
	}

#if defined(_DEBUG) || defined(DEBUG)
	spdlog::info("ContentManager의 DefualtContent가 준비되었습니다.");
#endif
}
