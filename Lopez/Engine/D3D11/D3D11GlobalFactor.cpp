#include "D3D11GlobalFactor.h"



JD3D11GlobalFactor& JD3D11GlobalFactor::GetInstance()
{
	static JD3D11GlobalFactor instance;
	return instance;
}

