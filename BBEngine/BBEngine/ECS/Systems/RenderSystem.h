#pragma once
#include "System.h"
#include "../ECSInclude.h"
#include "../ECSManager.h"
#include "../../GameUtils/Transform.h"
#include "../Renderer/VulkanRendererWindows/RenderInterface.h"
#include "../Components/Mesh.h"

extern ECS::ECSManager g_ECSManager;
namespace ECS {
	class RenderSystem : public System
	{
	public:
		void RenderObjects();
	};
}