#pragma once
#include "DataTypes/SharedRenderTypes.h"
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"
#include "Vector3.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& a_Gfx, BBE::GLTFFile* a_File, VertexShader* a_VertexShader, PixelShader* a_PixelShader);
	
	void AddToDraw(DirectX::XMMATRIX a_Transform);
	void Draw(Graphics& a_Gfx) noexcept override;
	void Update(float a_DeltaTime) noexcept override;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;

	struct InstanceBuffer {
		DirectX::XMMATRIX m_GameObjTransform;
	};

	std::vector<InstanceBuffer> m_InstanceBuffer;

	struct ModelNodes
	{
		uint32_t primitiveCount = 0;
		TransformBuf* transformBuf = nullptr;
		
		struct ModelPrimitive
		{
			VertexBuffer* vBuffer;
			Texture* m_Texture;
			Sampler* m_Sampler;
			IndexBuffer* m_IndexBuffer;

		}* primitives = nullptr;

	}* m_Nodes = nullptr;

	InputLayout* m_InputLayout;
	Topology* m_Topology;

	uint32_t m_nodeCount;
};