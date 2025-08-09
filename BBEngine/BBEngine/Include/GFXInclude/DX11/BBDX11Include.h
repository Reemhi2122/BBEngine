#pragma once
#include "Bindable/DX11ConstantBuffer.h"
#include "Bindable/DX11IndexBuffer.h"
#include "Bindable/DX11VertexBuffer.h"
#include "Bindable/DX11InputLayout.h"
#include "Bindable/DX11Sampler.h"
#include "Bindable/DX11Texture.h"
#include "Bindable/DX11Topology.h"
#include "Bindable/DX11CubeMap.h"

static DX11Texture* GFXCreateTexture()
{
	return new DX11Texture();
};

static DX11IndexBuffer* GFXCreateIndexBuffer()
{
	return new DX11IndexBuffer(); 
};

static DX11VertexBuffer* GFXCreateVertexBuffer()
{
	return new DX11VertexBuffer();
};

template<typename T>
static VertexConstantBuffer<T>* GFXCreateConstantBuffer()
{
	return new VertexConstantBuffer<T>();
};

static DX11CubeMap* GFXCreateCubeMap()
{
	return new DX11CubeMap();
};