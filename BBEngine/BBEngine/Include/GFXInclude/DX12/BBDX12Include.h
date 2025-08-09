#pragma once
#include "Bindable/DX12ConstantBuffer.h"
#include "Bindable/DX12IndexBuffer.h"
#include "Bindable/DX12VertexBuffer.h"
#include "Bindable/DX12Texture.h"
#include "Bindable/ICubeMap.h"

static DX12Texture* GFXCreateTexture()
{
	return new DX12Texture();
};

static DX12IndexBuffer* GFXCreateIndexBuffer()
{
	return new DX12IndexBuffer();
};

static DX12VertexBuffer* GFXCreateVertexBuffer()
{
	return new DX12VertexBuffer();
};

template<typename T>
static RootConstantBuffer<T>* GFXCreateConstantBuffer()
{
	return new RootConstantBuffer<T>();
};

static ICubeMap* GFXCreateCubeMap()
{
	return nullptr;
};