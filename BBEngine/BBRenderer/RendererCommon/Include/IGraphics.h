#pragma once
#include "Camera.h"
#include "RenderDefinitions.h"

//constexpr uint8_t FRAME_BUFFER_COUNT = 3;
#define FRAME_BUFFER_COUNT 3
#define FRAME_TEXTURE_TARGET 3
#define TOTAL_RENDER_TARGETS 6

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

	//Note(Stan): These should be in DX11 / DX12 specific grpahics classes
	//				But for now and testing I'm just hardcoding it here
	virtual BBRenderDevice* GetDevice() const noexcept = 0;

	virtual BBGraphicsCommandList* GetCommandList() const {};
	virtual uint8_t GetFrameCount() const {};
	virtual uint8_t GetCurrentFrame() const {};

	virtual void DrawIndexed(uint32_t a_IndexCount) = 0;

	//Note(Stan): DX11 specific
	virtual BBRenderContext* GetContext() const noexcept = 0;
	//End DX11 specific
};