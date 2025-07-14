#include "UserInterface.h"
#include "Graphics.h"
#include "System/FileHandler.h"
//#include "Bindable/Texture.h"

#include "GameLib/BBObject.h"

#include "BBEngine.h"
#include <shellapi.h>
#include <stack>

//Note(Stan): Prob move this to graphics.h
#define CUBEMAP_SIZE 6

namespace BBE
{
    namespace UI
    {
        void DrawHierarchy(Graphics& a_Graphics);
        void DrawInspector(Graphics& a_Graphics);
        void DrawAssetBrowser(Graphics& a_Graphics);

        void DrawRandomUI(Graphics& a_Graphics);

        static BBObject* g_InspectedObj = nullptr;
        static int selectedObjectNum = -1;
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

        //Note(Stan): Local objects reference, look into changing this?
        static std::vector<BBObject*>* m_GameObjectsPointer;

        //static Texture m_EmptyFolderTexture;
        //static Texture m_FileTexture;

        void InitializeUI(Graphics& a_Graphics, std::vector<BBObject*>* a_GameObjectListPointer)
        {
            //Note(Stan): Not a big fan of doing it like this, prob want to just request it from engine when needed.
            m_GameObjectsPointer = a_GameObjectListPointer;

            //m_EmptyFolderTexture = Texture(a_Graphics, "Assets/Image/Icons/closed_folder.png");
            //m_FileTexture = Texture(a_Graphics, "Assets/Image/Icons/file.png");
            ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        }

        std::string curPath = "Assets\\";
        std::stack<std::string> history;
        void UpdateUI(Graphics& a_Graphics)
        {
            bool open = true;

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::SetNextWindowBgAlpha(0.0f);

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &open, window_flags);
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            ImGui::End();
            ImGui::PopStyleVar(3);


            DrawHierarchy(a_Graphics);
            DrawAssetBrowser(a_Graphics);
            DrawInspector(a_Graphics);
            DrawRandomUI(a_Graphics);
        }

        void DrawHierarchy(Graphics& a_Graphics)
        {
            ImGui::Begin("Hierarchy");
            {
                for (uint32_t i = 0; i < m_GameObjectsPointer->size(); i++)
                {
                    ImGuiTreeNodeFlags flags = baseFlags;
                    if (i == selectedObjectNum)
                    {
                        flags |= ImGuiTreeNodeFlags_Selected;
                    }

                    ImGui::PushID(i);
                    bool isOpen = ImGui::TreeNodeEx("Hierarchy Object", flags, (*m_GameObjectsPointer)[i]->GetName());
                    if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                    {
                        g_InspectedObj = (*m_GameObjectsPointer)[i];
                        selectedObjectNum = i;
                    }

                    if (isOpen)
                    {
                        ImGui::Text("FakeChild1");
                        ImGui::Text("FakeChild2");
                        ImGui::TreePop();
                    }
                    ImGui::PopID();
                }
            }
            ImGui::End();
        }

        void DrawAssetBrowser(Graphics& a_Graphics)
        {
           /* ImGuiWindowFlags AssetBrowserFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse;
            if (ImGui::Begin("Assets Browser"), true, AssetBrowserFlags)
            {
                BBE::BBSystem::BBDIRECTORY dir;
                BBE::BBSystem::GetDirectoryInfo(curPath, &dir);

                if (ImGui::Button("Revert", ImVec2(100, 30)))
                {
                    if (history.size() > 0)
                    {
                        curPath = history.top();
                        history.pop();
                    }
                }
                ImGui::SameLine();
                ImGui::Text(curPath.c_str());

                ImGui::Spacing();

                const uint32_t imgSize = 64;
                ImGuiStyle& style = ImGui::GetStyle();
                float PanelSize = ImGui::GetWindowSize().x;
                uint32_t elementCountSameLine = std::max(std::floor(PanelSize / (imgSize + (style.ItemSpacing.x * 2))), 1.0f);

                uint32_t elementIndex = 0;

                for (uint32_t i = 0; i < dir.directoryCount; i++)
                {
                    ImGui::PushID(i);
                    if ((elementIndex % elementCountSameLine) != 0)
                        ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + imgSize);

                    if (ImGui::ImageButton("Folder", (ImTextureID)m_EmptyFolderTexture.GetRSV(), ImVec2(imgSize, imgSize)))
                    {
                        history.push(curPath);
                        curPath.append(dir.directories[i].name + "\\");
                    }

                    ImGui::TextWrapped(dir.directories[i].name.c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndGroup();
                    ImGui::PopID();

                    elementIndex++;
                }

                if ((elementIndex % elementCountSameLine) != 0)
                    ImGui::SameLine();

                for (uint32_t i = 0; i < dir.fileCount; i++)
                {
                    ImGui::PushID(i);
                    if ((elementIndex % elementCountSameLine) != 0)
                        ImGui::SameLine();

                    ImGui::BeginGroup();
                    ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + imgSize);

                    if (ImGui::ImageButton("Files", (ImTextureID)m_FileTexture.GetRSV(), ImVec2(imgSize, imgSize)))
                    {
                        std::string result(curPath + dir.files[i].name);
                        ShellExecute(0, 0, result.c_str(), 0, 0, SW_SHOW);
                    }

                    ImGui::TextWrapped(dir.files[i].name.c_str());
                    ImGui::PopTextWrapPos();
                    ImGui::EndGroup();
                    ImGui::PopID();

                    elementIndex++;
                }
            }
            ImGui::End();*/
        }

        void DrawInspector(Graphics& a_Graphics)
        {
            ImGui::Begin("Inspector");
            {
                if (g_InspectedObj)
                {
                    g_InspectedObj->InspectorDrawUI();
                }
            }
            ImGui::End();
        }

        void DrawRandomUI(Graphics& a_Graphics)
        {
            //ImGui::Begin("ShadowMapWindow");
            //{
            //    if (ImGui::TreeNode("Point Light"))
            //    {
            //        for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
            //        {
            //            ImGui::Image((ImTextureID)(void*)a_Graphics.m_TextureDepthSRV[i], ImVec2(200, 200));
            //        }
            //        ImGui::TreePop();
            //    }

            //    if (ImGui::TreeNode("Spot Light"))
            //    {
            //        ImGui::Image((ImTextureID)(void*)a_Graphics.m_SpotLightsDepthTest, ImVec2(200, 200));
            //        ImGui::TreePop();
            //    }

            //    if (ImGui::TreeNode("Directional Light"))
            //    {
            //        ImGui::Image((ImTextureID)(void*)a_Graphics.GetDirectionLightDepthMapRSV(), ImVec2(200, 200));
            //        ImGui::TreePop();
            //    }
            //}
            //ImGui::End();

            /*if (ImGui::Begin("Game Options"))
            {
                static int curVShader = 0;
                std::vector<const char*> vShaders;
                VertexShader* vShaderArray = a_Graphics.GetVertexShaderArray();
                for (uint32_t i = 0; i < 100; i++)
                {
                    if (vShaderArray[i].m_Path.empty()) break;
                    vShaders.push_back(vShaderArray[i].m_Path.c_str());
                }

                ImGui::Combo("VertexShaders", &curVShader, vShaders.data(), vShaders.size());
                if (ImGui::Button("Reload Vertex Shader"))
                {
                    a_Graphics.ReloadShader(ShaderType::VertexShader, curVShader);
                }

                std::vector<const char*> pShaders;
                PixelShader* pShadersArray = a_Graphics.GetPixelShaderArray();
                for (uint32_t i = 0; i < 100; i++)
                {
                    if (pShadersArray[i].m_Path.empty()) break;
                    pShaders.push_back(pShadersArray[i].m_Path.c_str());
                }

                static int curPShader = 0;
                ImGui::Combo("PixelShaders", &curPShader, pShaders.data(), pShaders.size());
                if (ImGui::Button("Reload Pixel Shader"))
                {
                    a_Graphics.ReloadShader(ShaderType::PixelShader, curPShader);
                }
            }
            ImGui::End();*/

            ImGui::Begin("GameWindow");
            {
                ImVec2 size = ImGui::GetWindowSize();
                ImGui::Image(a_Graphics.GetCurrentViewHandle().ptr, size);
                //ImGui::Image((ImTextureID)(void*)a_Graphics.GetGameViewRSV(), size);
            }
            ImGui::End();
        }
    }
}