#pragma once
#include "Camera.h"
#include "RenderDefinitions.h"

//constexpr uint8_t FRAME_BUFFER_COUNT = 3;
#define FRAME_BUFFER_COUNT 3
#define FRAME_TEXTURE_TARGET 3
#define TOTAL_RENDER_TARGETS 6

typedef uint32_t BBHandle;

enum class ShaderType
{
	VertexShader,
	PixelShader,
	GeometryShader,
	ComputeShader,
	HullShader
};

class IGraphics
{
public:
	virtual bool Initialize() = 0;
	virtual bool CloseInit() = 0;
	
	virtual void Update() = 0;
	
	virtual void StartFrame() = 0;
	virtual void Render() = 0;
	virtual void EndFrame() = 0;
	
	virtual void ShutDown() = 0;
	virtual void WaitForPreviousFrame() = 0;

	virtual Camera* GetCamera() const  = 0;
	virtual void SetCamera(Camera* a_Camera) = 0;


	virtual BBRenderDevice* GetDevice() const noexcept = 0;
	virtual BBGraphicsCommandList* GetCommandList() const { return nullptr; };
	virtual uint8_t GetFrameCount() const { return 0; };
	virtual uint8_t GetCurrentFrame() const { return 0; };

	virtual void DrawIndexed(uint32_t a_IndexCount) = 0;

	//Note(Stan): DX11 specific
	virtual BBRenderContext* GetContext() const noexcept { return nullptr; };
	//End DX11 specific

	//virtual BBHandle CreateShader(ShaderType a_Type, std::string a_Path, std::string a_EntryPointFunc = "main") = 0;
	//virtual void BindShader(ShaderType a_Type, BBHandle a_Shader) = 0;
	//virtual void ReloadShader(ShaderType a_Type, BBHandle a_Shader) = 0;
};