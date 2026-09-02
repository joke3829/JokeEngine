#include "JEngineShader.h"

ComPtr<ID3DBlob> CompileHLSL(LPCWCHAR pFileName, D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pTarget)
{
    ComPtr<ID3DBlob> blob{};
#if defined(_DEBUG) || defined(DEBUG)
    ComPtr<ID3DBlob> errorBlob{};
    HRESULT hr = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pTarget, 0, 0, blob.GetAddressOf(), errorBlob.GetAddressOf());
    if (FAILED(hr)) {
        std::wstring fname = pFileName;
        std::string sfname(fname.begin(), fname.end());
        if(errorBlob)
            spdlog::error("{0} Compile Failed - Error: {1}", sfname.c_str(), (const char*)errorBlob->GetBufferPointer());
        else
            spdlog::error("{0} Compile Failed - Error: ???", sfname.c_str());
        assert(0);
    }
#else
    ThrowIfFailed(D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pTarget, 0, 0, blob.GetAddressOf(), nullptr));
#endif

    return blob;
}
