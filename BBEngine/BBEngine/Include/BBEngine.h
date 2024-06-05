#pragma once
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"
#include "Drawable/Box.h"
#include "Drawable/Model.h"
#include "Drawable/DrawToTexture.h"
#include "GameLib/GameObject.h"

#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"

#include "Containers/FixedArray.h"
#include "Lights.h"

#include "Containers/Vector.h"

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
		void CalculateLightShadowMap();

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
		Vector<SpotLight> m_SpotLights;
		Vector<PointLight> m_PointLights;

		//Note(Stan):	I hate this but it should work
		Graphics& m_Graphics = m_Window.GetGraphics();

		Camera m_Cam1;
		Camera m_Cam2;

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}