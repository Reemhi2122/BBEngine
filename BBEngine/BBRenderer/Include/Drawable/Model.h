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

	void SetPosition(Vector3 a_Position);
	Vector3 GetPosition() const;

	void SetRotation(Vector3 a_Position);
	Vector3 GetRotation() const;

	void SetScale(Vector3 a_Position);
	Vector3 GetScale() const;

	const std::vector<Bindable*>& GetStaticBinds() const noexcept override
	{
		return std::vector<Bindable*>();
	}

private:
	Graphics* m_Graphics;

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

	Vector3 m_Scale = Vector3(1, 1, 1);
	Vector3 m_Rotation = Vector3(0, 0, 0);
	Vector3 m_Translation = Vector3(0, 0, 0);

	uint32_t m_nodeCount;
};

