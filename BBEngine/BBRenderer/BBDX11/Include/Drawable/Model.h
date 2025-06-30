//#pragma once
//#include "SharedRenderTypes.h"
//#include "Bindable/BindableInclude.h"
//#include "Drawable/DrawableBase.h"
//#include "Vector3.h"
//#include "Materials.h"
//
//struct ModelNodes
//{
//	uint32_t primitiveCount = 0;
//
//	Vector3 position;
//	Vector4 rotation;
//	Vector3 scale;
//
//	TransformBuf* transformBuf = nullptr;
//
//	struct ModelPrimitive
//	{
//		VertexBuffer* vBuffer;
//		Texture* m_Texture;
//		Sampler* m_Sampler;
//		IndexBuffer* m_IndexBuffer;
//		BBE::AlphaMode m_Blend;
//
//		Texture* m_KhrVolumeTexture;
//		Texture* m_KhrTransmissionTexture;
//
//		MaterialConstant m_MaterialConstant;
//	}*primitives = nullptr;
//};
//
//struct NodeContainer 
//{
//	ModelNodes* data;
//	uint32_t count;
//};
//
//class Model : public Drawable
//{
//public:
//	Model() = default;
//	Model(Graphics& a_Gfx, const char* a_Name, BBE::GLTFFile* a_File, uint32_t a_VertexShader, uint32_t a_PixelShader);
//	
//	void SetCurrentShader(uint32_t a_VertexShader, uint32_t a_PixelShader);
//	void ResetShaders();
//
//	void AddToDraw(DirectX::XMMATRIX a_Transform);
//	
//	void Draw(Graphics& a_Gfx) noexcept override;
//	void DrawInstanced(Graphics& a_Gfx, uint32_t a_InstanceCount) noexcept override;
//	void Update(float a_DeltaTime) noexcept override;
//
//	void SetTransform(DirectX::XMMATRIX* a_Transform) { m_CurTransform = a_Transform; }
//
//	NodeContainer GetNodes() { return { m_Nodes, m_nodeCount }; };
//
//	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
//	{
//		return std::vector<Bindable*>();
//	}
//
//private:
//	char m_Name[255] = "Undefined Model";
//
//	std::vector<DirectX::XMMATRIX> m_InstanceBuffer;
//	DirectX::XMMATRIX* m_CurTransform = nullptr;
//
//	ModelNodes* m_Nodes = nullptr;
//
//	InputLayout* m_InputLayout;
//	Topology* m_Topology;
//
//	uint32_t m_VertexShader;
//	uint32_t m_PixelShader;
//
//	uint32_t m_CurVertexShader;
//	uint32_t m_CurPixelShader;
//
//	PixelConstantBuffer<MaterialConstant> m_ModelPixelBuffer;
//
//	uint32_t m_nodeCount;
//};