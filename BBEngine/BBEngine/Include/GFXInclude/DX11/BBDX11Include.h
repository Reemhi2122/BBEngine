#pragma once
#include "Bindable/DX11ConstantBuffer.h"
#include "Bindable/DX11IndexBuffer.h"
#include "Bindable/DX11VertexBuffer.h"
#include "Bindable/DX11InputLayout.h"
#include "Bindable/DX11Sampler.h"
#include "Bindable/DX11Texture.h"
#include "Bindable/DX11Topology.h"

DX11Texture* GFXCreateTexture()
{
	return new DX11Texture();
};

DX11IndexBuffer* GFXCreateIndexBuffer()
{
	return new DX11IndexBuffer(); 
};

DX11VertexBuffer* GFXCreateVertexBuffer()
{
	return new DX11VertexBuffer();
};

template<typename T>
VertexConstantBuffer<T>* GFXCreateConstantBuffer()
{
	return new VertexConstantBuffer<T>();
};