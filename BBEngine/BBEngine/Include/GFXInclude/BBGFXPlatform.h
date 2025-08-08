#pragma once
#ifdef BBDX11
#include "DX11/BBDX11Include.h"
#elif BBDX12
#include "DX12/BBDX12Include.h"
#else
void* GFXCreateTexture()
{
	return nullptr;
};

void* GFXCreateIndexBuffer()
{
	return nullptr;
};

void* GFXCreateVertexBuffer()
{
	return nullptr;
};

template
void* GFXCreateConstantBuffer()
{
	return nullptr;
};
#endif