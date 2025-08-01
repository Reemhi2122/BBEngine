#pragma once
#include "GameLib/Drawable/Model.h"
#include "GameLib/BBComponent.h"
#include "GameLib/Components/TransformComponent.h"

namespace BBE 
{
	class MeshComponent : public BBComponent
	{
	public:
		MeshComponent(IGraphics& a_Graphics, Model* a_Model, uint32_t a_NodeIndex, TransformComponent* a_Transform);
		~MeshComponent() = default;

		virtual void Update(IGraphics& a_Graphics) override;
		virtual void Draw(IGraphics& a_Graphics) override;

		virtual void InspectorDrawUI() override;

		Model* GetModel() const noexcept { return m_Model; };
		bool SetModel(Model* a_Model);

	private:
		uint32_t m_NodeIndex;
		Model* m_Model;
		TransformComponent* m_Transform;
	};
}