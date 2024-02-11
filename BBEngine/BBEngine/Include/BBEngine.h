#pragma once
#include "BBRenderer.h"
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"

#include "Drawable/Box.h"
#include "Drawable/Model.h"
#include "DirectionalLight.h"

namespace BBE {
	
	struct cbPerFrame {
		DirectionalLight Light;
	};

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

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}