#pragma once
#include "BBRenderer.h"
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"
#pragma once
#include "Drawable/Box.h"
#include "Drawable/Model.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

struct cbPerFrame {
	DirectionalLight directionallight;
	SpotLight spotlight;
};

namespace BBE {
	class BBEngine {
	public:
		BBEngine();
		~BBEngine();

		int StartBBEngine();
		void Start();

	private:
		void Update();
		void CheckInput();

	private:
		BBWindow m_Window;
		BBTimer m_Timer;
		BBWinRenderer m_BBWinRenderer;

		std::vector<Box*> m_Boxes;
		std::vector<Model*> m_Model;

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		PixelConstantBuffer<cbPerFrame> m_PerFrameBuffer;

		DirectionalLight m_DirectionalLight;
		SpotLight m_SpotLight;

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}