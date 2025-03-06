#pragma once
#include "Drawable/Model.h"
#include "GameLib/BBComponent.h"
#include "GameLib/Components/Transform.h"

namespace BBE 
{
	class GameObject : public BBComponent
	{
	public:
		GameObject(Graphics& a_Graphics, Model * a_Model, Transform* a_Transform);
		~GameObject() = default;

		virtual void Update(Graphics& a_Graphics) override;
		virtual void Draw(Graphics& a_Graphics) override;

		virtual void InspectorDrawUI() override;

		Model* GetModel() const noexcept { return m_Model; };

	private:
		Model* m_Model;
		Transform* m_Transform;
	};
}