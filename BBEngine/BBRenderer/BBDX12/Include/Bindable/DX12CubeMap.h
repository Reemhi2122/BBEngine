#pragma once
#include "Bindable/ICubeMap.h"

class DX12CubeMap : public ICubeMap
{
public:
	DX12CubeMap() = default;
	~DX12CubeMap() = default;

	bool Create();


private:
};