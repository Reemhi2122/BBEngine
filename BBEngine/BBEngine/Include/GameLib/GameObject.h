#pragma once
#include "Drawable/Model.h"

namespace BBE 
{
	class GameObject 
	{
	public:
		GameObject(Graphics& a_Graphics, char* a_Name, Model * a_Model, Vector3 a_Position = Vector3(0, 0, 0), Vector3 a_Rotation = Vector3(0, 0, 0), Vector3 a_Scale = Vector3(1, 1, 1));
		~GameObject();

		virtual void Update(Graphics& a_Graphics);
		virtual void Draw(Graphics& a_Graphics);

		void UpdateTransform() noexcept;
	
		const char* GetName() { return m_ObjName; }

		void SetPosition(Vector3 a_Position);
		Vector3 GetPosition() const;
		Vector3& GetPositionRef() { return m_Position; };

		void SetRotation(Vector3 a_Position);
		Vector3 GetRotation() const;
		Vector3& GetRotationRef() { return m_Rotation; };

		void SetScale(Vector3 a_Position);
		Vector3 GetScale() const;
		Vector3& GetScaleRef() { return m_Scale; };

		Model* GetModel() const noexcept { return m_Model; };

	private:
		Model* m_Model;

		char m_ObjName[255];

		DirectX::XMMATRIX m_Transform;

		Vector3 m_Position;
		Vector3 m_Rotation;
		Vector3 m_Scale;
	};
}