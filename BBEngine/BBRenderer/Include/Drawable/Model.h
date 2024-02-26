#include "DataTypes/SharedRenderTypes.h"
#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"
#include "Vector3.h"
#include "DirectionalLight.h"

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& a_Gfx, BBE::GLTFFile* a_File, VertexShader* a_VertexShader, PixelShader* a_PixelShader);
	
	void Draw(Graphics& a_Gfx) noexcept override;

	void Update(float a_DeltaTime) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;

	struct ModelPrimitive {
	
		VertexBuffer* vBuffer;
		Texture* m_Texture;
		Sampler* m_Sampler;
		IndexBuffer* m_IndexBuffer;

	}* m_Primitives;

	InputLayout* m_InputLayout;
	Topology* m_Topology;
	TransformBuf* m_TransformBuf;

	float m_Angle;
	Vector3 m_Translation;

	uint32_t m_PrimitiveCount;
};