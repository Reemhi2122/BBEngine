#pragma once
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"

#include "GameLib/BBObject.h"

#include "Containers/FixedArray.h"
#include "Containers/Vector.h"

#include "Camera.h"

#include "FileLoaders/Models/GLTFParser.h"

//Graphics includes
#include "Gamelib/Drawable/Skybox.h"
//#include "Drawable/DrawToTexture.h"
#include "GameLib/Drawable/Model.h"
//#include "Drawable/Quad.h"
//
//#include "DepthStencil.h"

#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"

//#include "Lights.h"
//#include "Vertexconstant.h"

class DX11InputLayout;
class DX11Topology;

namespace BBE {
	class BBEngine {
	public:
		BBEngine();
		~BBEngine();

		int StartBBEngine();
		void Initialize();

	private:
		void Update();
		void CheckInput();

		void DrawAllModels();

		void DrawUI();
		void RenderDebugOptions();

		//void CalculateLightShadowMapDirectionalLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader);
		//void CalculateLightShadowMapSpotLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, uint32_t a_Index);
		//void CalculateLightShadowMapPointLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, PointLight a_Spotlight, uint32_t a_Index);

		std::vector<BBObject*>* GetGameObjects() { return &m_GameObjects; };

	private:
		BBWindow m_Window;
		BBTimer m_Timer;

		std::vector<BBObject*> m_GameObjects;
		std::vector<BBObject*> m_RootObjects;
		std::vector<Model*> m_Models;
		//DrawToTexture* m_DrawToTexture;

		uint32_t m_BaseVertexShader;
		uint32_t m_BasePixelShader;

		uint32_t m_VertexShader;
		uint32_t m_PixelShader;

		uint32_t m_RTTVertexShader;
		uint32_t m_RTTPixelShader;

		uint32_t m_VSShadowMapShader;
		uint32_t m_PSShadowMapShader;
		uint32_t m_PSSpotLightShadowMapShader;

		DX11InputLayout* m_InputLayout;
		DX11Topology* m_Topology;

		//PixelConstantBuffer<cbPerFrame> m_PerFrameBuffer;
		//PixelConstantBuffer<ShadowMapCreation> m_ShadowMapCB;
		//ShadowMapCreation m_ShadowMapCBBuffer;

		//VertexConstantBuffer<vcbPerFrame> m_LightMatrix;

		GLTFFile m_SponzaFile;
		GLTFFile m_LanternFile;
		GLTFFile m_CarFile;
		GLTFFile m_ABeautifulGameFile;
		GLTFFile m_GlassVase;

		//DirectionalLight m_DirectionalLight;
		//Vector<SpotLight> m_SpotLights;
		//Vector<PointLight> m_PointLights;

		//Quad* m_Quad;

		//Note(Stan):	I hate this but it should work
		Graphics& m_Graphics = m_Window.GetGraphics();

		Camera m_Cam1;
		Camera m_Cam2;

		Skybox* m_Skybox;

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}