#include "JObjectRegistry.h"

bool JObjectRegistry::Register(const char* ObjectType, CreateFunc func)
{
    auto& map = GetMap();
    auto [it, inserted] = map.try_emplace(ObjectType, func);
    if (inserted) {
        auto& keys = GetKeys();
        keys.emplace_back(ObjectType);
        std::sort(keys.begin(), keys.end());
    }
#if DEV_MODE
    else spdlog::error("JObjectRegistry::Register Failed: {}", ObjectType);
#endif

    return inserted;
}

std::shared_ptr<JObject> JObjectRegistry::CreateObject(const char* ObjectType, std::vector<XMFLOAT4X4>& vWorld, std::vector<XMFLOAT4X4>& vWorldTP, UINT nodeIndex, const char* name)
{
    auto& map = GetMap();
    if (not map.contains(ObjectType)) {
#if DEV_MODE
        spdlog::error("{} not contains JObjectRegistry", ObjectType);
#endif
        return nullptr;
    }
    return map[ObjectType](vWorld, vWorldTP, nodeIndex, name);
}

const std::vector<std::string>& JObjectRegistry::GetRegisterdObjectNames()
{
    return GetKeys();
}

