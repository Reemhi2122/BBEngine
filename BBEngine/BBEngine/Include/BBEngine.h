#pragma once
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"
#pragma once
#include "Drawable/Box.h"
#include "Drawable/Model.h"
#include "Drawable/DrawToTexture.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "GameLib/GameObject.h"

struct cbPerFrame {
	DirectionalLight directionallight;
	PointLight spotlight;
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

		//Note(Stan):	This should be temp
		void RenderToTexture();

	private:
		BBWindow m_Window;
		BBTimer m_Timer;

		std::vector<GameObject*> m_GameObjects;
		std::vector<Model*> m_Models;
		DrawToTexture* m_DrawToTexture;

		VertexShader m_VertexShader;
		PixelShader m_PixelShader;

		VertexShader m_RTTVertexShader;
		PixelShader m_RTTPixelShader;

		PixelConstantBuffer<cbPerFrame> m_PerFrameBuffer;

		GLTFFile m_SponzaFile;
		GLTFFile m_LanternFile;
		GLTFFile m_CarFile;
		GLTFFile m_FoxFile;
		GLTFFile m_ABeautifulGameFile;

		DirectionalLight m_DirectionalLight;
		PointLight m_PointLight;

		//Note(Stan):	I hate this but it should work
		Graphics& m_Graphics = m_Window.GetGraphics();

		Camera m_Cam1;
		Camera m_Cam2;

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}