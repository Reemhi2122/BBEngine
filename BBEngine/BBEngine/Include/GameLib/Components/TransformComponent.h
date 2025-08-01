#pragma once
#include "GameLib/BBComponent.h"
#include "GameLib/TransformBuf.h"
#include "Vector3.h"

class ModelTransform;

class TransformComponent : public BBComponent
{
public:
	TransformComponent(IGraphics& a_Graphics, Vector3 a_Position = Vector3(0, 0, 0), Vector3 a_Rotation = Vector3(0, 0, 0), Vector3 a_Scale = Vector3(1, 1, 1));
	TransformComponent(IGraphics& a_Graphics, ModelTransform* a_ModelTransform, Vector3 a_Pos, Vector3 a_Rot, Vector3 a_Scale, TransformComponent* a_ParentTransform);
	~TransformComponent() = default;

	void Update(IGraphics& a_Graphics) override;
	void Draw(IGraphics& a_Graphics) override {};

	void InspectorDrawUI() override;

	TransformBuf* GetTransformBuf() { return m_TransformBuf; };

	void SetPosition(Vector3 a_Position);
	Vector3 GetPosition() const { return m_Position; };
	Vector3& GetPositionRef() { return m_Position; };

	void SetRotation(Vector3 a_Position);
	Vector3 GetRotation() const { return m_Rotation; };
	Vector3& GetRotationRef() { return m_Rotation; };

	void SetScale(Vector3 a_Position);
	Vector3 GetScale() const { return m_Scale; };
	Vector3& GetScaleRef() { return m_Scale; };

private:
	Vector3 m_Position;
	Vector3 m_Rotation;
	Vector3 m_Scale;

	TransformBuf* m_TransformBuf = nullptr;
	TransformComponent* m_ParentTransform = nullptr;
};