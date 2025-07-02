#pragma once
#include "GameLib/Drawable/Model.h"
#include "GameLib/BBComponent.h"
#include "GameLib/Components/Transform.h"

namespace BBE 
{
	class MeshComponent : public BBComponent
	{
	public:
		MeshComponent(IGraphics& a_Graphics, Model* a_Model, Transform* a_Transform);
		~MeshComponent() = default;

		virtual void Update(IGraphics& a_Graphics) override;
		virtual void Draw(IGraphics& a_Graphics) override;

		virtual void InspectorDrawUI() override;

		Model* GetModel() const noexcept { return m_Model; };
		bool SetModel(Model* a_Model);

	private:
		Model* m_Model;
		Transform* m_Transform;
	};
}