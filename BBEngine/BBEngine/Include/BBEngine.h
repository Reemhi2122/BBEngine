#pragma once
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"
#pragma once
#include "Drawable/Box.h"
#include "Drawable/Model.h"
#include "Drawable/DrawToTexture.h"
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

		std::vector<Box*> m_Boxes;
		std::vector<Model*> m_Model;
		DrawToTexture* m_DrawToTexture;

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		VertexShader m_RTTVertexShader;
		PixelShader m_RTTPixelShader;

		PixelConstantBuffer<cbPerFrame> m_PerFrameBuffer;

		DirectionalLight m_DirectionalLight;
		SpotLight m_SpotLight;

		//Note(Stan):	I hate this but it should work
		Graphics& m_Graphics = m_Window.GetGraphics();

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}