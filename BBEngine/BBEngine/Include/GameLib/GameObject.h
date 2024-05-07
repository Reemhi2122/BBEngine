#pragma once
#include "Drawable/Model.h"

namespace BBE 
{
	class GameObject 
	{
	public:
		GameObject(Graphics& a_Graphics, Model* a_Model, Vector3 a_Position = Vector3(0, 0, 0), Vector3 a_Rotation = Vector3(0, 0, 0), Vector3 a_Scale = Vector3(1, 1, 1));
		~GameObject();

		virtual void Update(Graphics& a_Graphics);

		DirectX::XMMATRIX GetTransformXM(Graphics& a_Graphics) const noexcept;
	
		void SetPosition(Vector3 a_Position);
		Vector3 GetPosition() const;

		void SetRotation(Vector3 a_Position);
		Vector3 GetRotation() const;

		void SetScale(Vector3 a_Position);
		Vector3 GetScale() const;

	private:
		Model* m_Model;

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;
	};
}