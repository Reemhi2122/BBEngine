#pragma once
#include "SharedRenderTypes.h"
//#include "Bindable/BindableInclude.h"
//#include "Drawable/DrawableBase.h"

#include "GameLib/TransformBuf.h"
#include "Bindable/DX12IndexBuffer.h"
#include "Bindable/DX12VertexBuffer.h"
#include "Bindable/DX12Texture.h"

#include "Drawable.h"

#include "Vector3.h"
#include "Materials.h"

struct ModelTransform
{
	Vector3 position;
	Vector4 rotation;
	Vector3 scale;
};

struct ModelNode
{
	char name[255] = "Undefined Model";
	uint32_t primitiveCount = 0;

	ModelTransform objectTransform;

	ModelNode* parent;
	std::vector<ModelNode*> children;
	
	struct ModelPrimitive
	{
		IVertexBuffer* vBuffer;
		ITexture* m_Texture;
		//Sampler* m_Sampler;
		IIndexBuffer* m_IndexBuffer;
		BBE::AlphaMode m_Blend;

		ITexture* m_KhrVolumeTexture;
		ITexture* m_KhrTransmissionTexture;

		MaterialConstant m_MaterialConstant;
	}*primitives = nullptr;
};

struct NodeContainer
{
	ModelNode* data;
	uint32_t count;
};

class Model : public Drawable
{
public:
	Model() = default;
	Model(IGraphics& a_Gfx, const char* a_Name, BBE::GLTFFile* a_File, uint32_t a_VertexShader, uint32_t a_PixelShader);

	void SetCurrentShader(uint32_t a_VertexShader, uint32_t a_PixelShader);
	void ResetShaders();

	void AddToDraw(DirectX::XMMATRIX a_Transform);

	void Draw(IGraphics& a_Gfx, uint32_t a_NodeIndex) noexcept override;
	void DrawInstanced(IGraphics& a_Gfx, uint32_t a_InstanceCount) noexcept override;
	void Update(float a_DeltaTime) noexcept override;

	void SetTransform(TransformBuf* a_TransformBuf) { m_CurTransform = a_TransformBuf; }

	NodeContainer GetNodes() { return { m_Nodes, m_nodeCount }; };

	//const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	//{
	//	return std::vector<Bindable*>();
	//}

private:
	char m_Name[255] = "Undefined Model";

	std::vector<DirectX::XMMATRIX> m_InstanceBuffer;
	TransformBuf* m_CurTransform = nullptr;

	ModelNode* m_Nodes = nullptr;

	//InputLayout* m_InputLayout;
	//Topology* m_Topology;

	uint32_t m_VertexShader;
	uint32_t m_PixelShader;

	uint32_t m_CurVertexShader;
	uint32_t m_CurPixelShader;

	IConstantBuffer<MaterialConstant>* m_ModelPixelBuffer;

	uint32_t m_nodeCount;
};