#include "DataTypes/SharedRenderTypes.h"
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"
#include "Vector3.h"
#include "DirectionalLight.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& a_Gfx, BBE::Mesh::Primative a_ModelFile, BBE::GLTFFile* a_File, VertexShader* a_VertexShader, PixelShader* a_PixelShader);
	
	void Update(float a_DeltaTime) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;

	VertexBuffer* vBuffer;
	IndexBuffer* IBuffer;
	//VertexShader* vShader;
	//PixelShader* pShader;
	InputLayout* m_InputLayout;
	Topology* m_Topology;
	TransformBuf* m_TransformBuf;
	Texture* m_Texture;
	Sampler* m_Sampler;

	float m_Angle;
	Vector3 m_Translation;
};