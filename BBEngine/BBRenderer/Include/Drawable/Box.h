#pragma once
#include "Drawable/DrawableBase.h"
#include <random>
#include "Bindable/BindableInclude.h"

struct ConstantBufferColor {
	struct {
		float r;
		float g;
		float b;
		float a;
	}face_colors[6];
};

class Box : public DrawableBase<Box> {

public:
	Box(Graphics& a_Gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist);

	void Update(float a_DeltaTime) noexcept override;

private:
	VertexBuffer*	vBuffer;
	IndexBuffer*	IBuffer;
	VertexShader*	vShader;
	PixelShader*	pShader;
	InputLayout*	m_InputLayout;
	Topology*		m_Topology;
	TransformBuf*	m_TransformBuf;
	PixelConstantBuffer<ConstantBufferColor>* cCB;
	
	float m_R;
	float m_Roll;
	float m_Pitch;
	float m_Yaw;
	float m_Theta;
	float m_Phi;
	float m_Chi;

	float m_DRoll;
	float m_DPitch;
	float m_DYaw;
	float m_DTheta;
	float m_DPhi;
	float m_DChi;
};