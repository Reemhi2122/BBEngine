#pragma once
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"

#include "GameLib/BBObject.h"

#include "Containers/FixedArray.h"
#include "Containers/Vector.h"

//Graphics includes
#include "Drawable/Skybox.h"
#include "Drawable/DrawToTexture.h"
#include "Drawable/Model.h"

#include "DepthStencil.h"

#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"
#include "Lights/DirectionalLight.h"

#include "Lights.h"
#include "Vertexconstant.h"


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

		//Note(Stan):	This should be temp
		void RenderToTexture();
		void DrawAllModels();

		void DrawUI();
		void RenderDebugOptions();

		void CalculateLightShadowMapDirectionalLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader);
		void CalculateLightShadowMapSpotLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, uint32_t a_Index);
		void CalculateLightShadowMapPointLight(std::vector<BBObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, PointLight a_Spotlight, uint32_t a_Index);

		std::vector<BBObject*>* GetGameObjects() { return &m_GameObjects; };

	private:
		BBWindow m_Window;
		BBTimer m_Timer;

		std::vector<BBObject*> m_GameObjects;
		std::vector<Model*> m_Models;
		DrawToTexture* m_DrawToTexture;

		uint32_t m_VertexShader;
		uint32_t m_PixelShader;

		uint32_t m_RTTVertexShader;
		uint32_t m_RTTPixelShader;

		uint32_t m_VSShadowMapShader;
		uint32_t m_PSShadowMapShader;
		uint32_t m_PSSpotLightShadowMapShader;

		PixelConstantBuffer<cbPerFrame> m_PerFrameBuffer;
		PixelConstantBuffer<ShadowMapCreation> m_ShadowMapCB;
		ShadowMapCreation m_ShadowMapCBBuffer;

		VertexConstantBuffer<vcbPerFrame> m_LightMatrix;

		//Note(Stan): Fix this somewhere
		ID3D11DepthStencilView* m_PLTextureDepthStencilViews[20][6];
		ID3D11DepthStencilView* m_SLTextureDepthStencilViews[120];
		ID3D11DepthStencilView* m_DLTextureDepthStencilView;

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

		Skybox* m_Skybox;

		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}