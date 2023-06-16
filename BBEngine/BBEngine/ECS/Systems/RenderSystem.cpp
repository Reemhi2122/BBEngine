#include "RenderSystem.h"

namespace ECS {
	void RenderSystem::RenderObjects()
	{
		for (Entity t_Entity : m_Entities) {
			Mesh t_Mesh = g_ECSManager.GetComponent<Mesh>(t_Entity);

			if (t_Mesh.m_Render) {
				RLEngine::Transform t_Transform = g_ECSManager.GetComponent<RLEngine::Transform>(t_Entity);
				Render::RenderInterface::DrawObject(t_Transform.GetModelMatrix(), t_Mesh.m_MeshHandle, t_Mesh.m_MaterialHandle);
			}
		}
	}
}