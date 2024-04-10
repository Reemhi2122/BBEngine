#pragma once
#include "Drawable/Model.h"

namespace BBE 
{
	class GameObject 
	{
	public:
		GameObject(Model* a_Model, Vector3 a_Position = Vector3(0, 0, 0), Vector3 a_Rotation = Vector3(0, 0, 0), Vector3 a_Scale = Vector3(1, 1, 1));
		~GameObject();

		virtual void Update(Graphics& a_Graphics);

		DirectX::XMMATRIX GetTransformXM() const noexcept;
	
	private:
		Model* m_Model;

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;
	};
}