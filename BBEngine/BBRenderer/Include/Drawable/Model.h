#pragma once
#include "DataTypes/SharedRenderTypes.h"
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"
#include "Vector3.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& a_Gfx, BBE::GLTFFile* a_File, uint32_t a_VertexShader, uint32_t a_PixelShader);
	
	void SetCurrentShader(uint32_t a_VertexShader, uint32_t a_PixelShader);
	void ResetShaders();
	void AddToDraw(DirectX::XMMATRIX a_Transform);
	
	void Draw(Graphics& a_Gfx) noexcept override;
	void Update(float a_DeltaTime) noexcept override;


	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:

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

	uint32_t m_VertexShader;
	uint32_t m_PixelShader;

	uint32_t m_CurVertexShader;
	uint32_t m_CurPixelShader;

	uint32_t m_nodeCount;
};