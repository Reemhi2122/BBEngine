#include "Bindable/BindableInclude.h"
#include "Drawable/DrawableBase.h"

//TODO(Stan): Probably need to remove or move this
struct ConstantBufferColorModel
{
	struct
	{
		float r;
		float g;
		float b;
		float a;
	}face_colors[1];
};

class Model : public Drawable
{
public:
	Model() = default;
	Model(Graphics& a_Gfx, Vertex* ver, uint32_t vertAmount, unsigned short* indices, uint32_t indicesAmount);
	
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
	VertexShader* vShader;
	PixelShader* pShader;
	InputLayout* m_InputLayout;
	Topology* m_Topology;
	TransformBuf* m_TransformBuf;
	PixelConstantBuffer<ConstantBufferColorModel>* cCB;

	float m_Angle;
};