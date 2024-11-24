#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"
#include "FileLoaders/Models/GLTFParser.h"
#include "Vector4.h"

#include <chrono>
#include <iostream>
#include <cstdint>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include "Camera.h"
#include "FileLoaders/Image/BMPLoader.h"

namespace BBE
{
    TaskDesc* testdesc = nullptr;

    BBEngine::BBEngine()
        : m_Window(1600, 900, "BBWindow test")
    {
        BB_Log_Init("BBLogger", LOG_ALL, "logs/");
        m_ArenaAllocator.Init(BBE::PageSize);
        m_StackAllocator.Init(128 * BBE::MBSize);
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(8, 2);
    }

    BBEngine::~BBEngine()
    {
        BBFree(m_ArenaAllocator, m_ThreadPool);
    }

    int BBEngine::StartBBEngine()
    {
        Initialize();

        try
        {
            for (;;)
            {
                if (const auto ecode = BBWindow::ProcessMessages())
                    return *ecode;

                Update();
            }
        }
        catch (const BBException& e)
        {
            MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
        }
        catch (const std::exception& e)
        {
            MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        catch (...)
        {
            MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        return -1;
    }

    void BBEngine::Initialize()
    {
        GLTFParser parser;
        parser.Parse("Assets/Models/Sponza/Sponza/", "Sponza.gltf", &m_SponzaFile);
        parser.Parse("Assets/Models/Lantern/glTF/", "Lantern.gltf", &m_LanternFile);
        parser.Parse("Assets/Models/ToyCar/glTF/", "ToyCar.gltf", &m_CarFile);
        parser.Parse("Assets/Models/ABeautifulGame/glTF/", "ABeautifulGame.gltf", &m_ABeautifulGameFile);

        m_Graphics.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 100.f));
        m_Window.m_Keyboard.EnableAutorepeat();

        m_DirectionalLight = DirectionalLight(
            Vector3(0.25f, 0.5f, -1.0f),
            Vector4(0.1f, 0.1f, 0.1f, 1.0f),
            Vector4(0.5f, 0.5f, 0.5f, 1.0f)
        );

        m_PointLights.Push_Back(PointLight(
            Vector3(-5.0f, 2.0f, 0.0f),
            Vector3(0.0f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            1000.0f
        ));

        m_PointLights.Push_Back(PointLight(
            Vector3(6.0f, 2.0f, 0.0f),
            Vector3(0.0f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            1000.0f
        ));

        //m_SpotLights.Push_Back(SpotLight(
        //    Vector3(8.0f, 2.0f, 0.0f),
        //    Vector3(-1.0f, 0.0f, 0.0f),
        //    5.f,
        //    Vector3(0.4f, 0.2f, 0.0f),
        //    Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        //    Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        //    1000.0f
        //));

        m_Graphics.CreatePointLightDepthCubeMapArray();
        for (uint32_t i = 0; i < m_PointLights.Size(); i++)
        {
            m_Graphics.CreatePointLightDepthCubeMap(/*m_PointLights[i].*/m_PLTextureDepthStencilViews[i], i);
        }

        m_Graphics.CreateSpotLightDepthMapArray();
        for (uint32_t i = 0; i < m_SpotLights.Size(); i++)
        {
            m_Graphics.CreateSpotLightDepthTexture(m_SLTextureDepthStencilViews[i], i);
        }
        
        m_PerFrameBuffer = PixelConstantBuffer<cbPerFrame>(m_Graphics);
        m_PerFrameBuffer.Bind(m_Graphics);

        m_ShadowMapCB = PixelConstantBuffer<ShadowMapCreation>(m_Graphics, 1, 1);
        m_ShadowMapCB.Bind(m_Graphics);

        m_VertexShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VertexShader.hlsl");
        m_PixelShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PixelShader.hlsl");

        m_RTTVertexShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VSDrawToTexture.hlsl");
        m_RTTPixelShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PSDrawToTexture.hlsl");

        m_VSShadowMapShader = m_Graphics.CreateShader(ShaderType::VertexShader, "Assets/VSShadowMap.hlsl");
        m_PSShadowMapShader = m_Graphics.CreateShader(ShaderType::PixelShader, "Assets/PSShadowMap.hlsl");

        Model* Sponza = BBNew(m_StackAllocator, Model)(m_Graphics, &m_SponzaFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(Sponza);
        Model* lantern = BBNew(m_StackAllocator, Model)(m_Graphics, &m_LanternFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(lantern);
        //Model* car = BBNew(m_StackAllocator, Model)(m_Graphics, &m_CarFile, m_VertexShader, m_PixelShader);
        //m_Models.push_back(car);
        Model* aBeautifulGame = BBNew(m_StackAllocator, Model)(m_Graphics, &m_ABeautifulGameFile, m_VertexShader, m_PixelShader);
        m_Models.push_back(aBeautifulGame);

        m_Skybox = BBNew(m_StackAllocator, Skybox)(m_Graphics);

        int XSize = 2, YSize = 2;
        for (size_t i = 0; i < XSize; i++) {
            for (size_t y = 0; y < YSize; y++) {
                GameObject* sponzaObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, Sponza, Vector3(i * 50, 0, y * 50));
                m_GameObjects.push_back(sponzaObj);
            }
        }

        GameObject* lanternObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, lantern, Vector3(-3, 0, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        m_GameObjects.push_back(lanternObj);

        //GameObject* carObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, car, Vector3(0, 2, 0), Vector3(0, 0, 0), Vector3(10, 10, 10));
        //m_GameObjects.push_back(carObj);

        GameObject* beautifulGameObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, aBeautifulGame, Vector3(0, 2, -25), Vector3(0, 0, 0), Vector3(10, 10, 10));
        m_GameObjects.push_back(beautifulGameObj);

        //Cameras
        m_Cam1.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Cam2.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Cam2.SetViewPort(1024, 1024);
        m_Graphics.SetCamera(&m_Cam1);
    }

    bool show_demo_window = true;
    float incr = 0;
    void BBEngine::Update()
    {
        m_Graphics.ClearBuffer(0.07f, 0.0f, 0.012f);
        m_Skybox->Draw(m_Graphics);

        CheckInput();

        cbPerFrame FrameConstantBuffer;
        FrameConstantBuffer.directionalLight = m_DirectionalLight;

        //m_PointLights[0].position.x = (sin(incr += 0.001f) * 5);
        //m_GameObjects[5]->SetPosition(Vector3(3 + sin(incr += 0.01), 0, 0));

        for (uint32_t i = 0; i < m_SpotLights.Size(); i++) {
            FrameConstantBuffer.spotlights[i] = m_SpotLights[i];
            CalculateLightShadowMap(m_GameObjects, m_VSShadowMapShader, m_PSShadowMapShader, i);
        }
        
        for (uint32_t i = 0; i < m_PointLights.Size(); i++) {
            FrameConstantBuffer.pointlights[i] = m_PointLights[i];
            CalculateLightShadowMapSpotLight(m_GameObjects, m_VSShadowMapShader, m_PSShadowMapShader, m_PointLights[i], i);
        }

        m_PerFrameBuffer.Update(m_Graphics, FrameConstantBuffer);

        for (size_t i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->Update(m_Graphics);
        }
       
        m_Graphics.BindDepthSampler();
        m_Graphics.BindDepthTexture(m_Graphics.GetPointLightDepthCubeArrayRSV(), 2, 1);

        for (size_t i = 0; i < m_GameObjects.size(); i++) {
            m_GameObjects[i]->Draw(m_Graphics);
        }

        m_Graphics.UnbindSRV(1);
        m_Graphics.UnbindSRV(2);

        RenderDebugOptions();
        DrawUI();

        m_Graphics.EndFrame();
    }

    void BBEngine::DrawAllModels()
    {
        for (uint32_t i = 0; i < m_Models.size(); i++)
        {
            m_Models[i]->Draw(m_Graphics);
        }
    }

    void BBEngine::DrawUI()
    {
        ImGui::Begin("GameWindow");
        {
            for (uint32_t i = 0; i < CUBEMAP_SIZE; i++)
            {
                ImGui::Image((void*)m_Graphics.m_TextureDepthSRV[i], ImVec2(200, 200));
            }
        }
        ImGui::End();

        ImGui::Begin("Objects");
        {
            for (uint32_t i = 0; i < m_GameObjects.size(); i++)
            {
                char name[8];
                itoa(i, name, 10);
                if (ImGui::CollapsingHeader(name))
                {
                    ImGui::InputFloat3("Transform", m_GameObjects[i]->GetPositionRef().GetXYZ());
                    ImGui::InputFloat3("Rotation",  m_GameObjects[i]->GetRotationRef().GetXYZ());
                    ImGui::InputFloat3("Scale",     m_GameObjects[i]->GetScaleRef().GetXYZ());

                    NodeContainer con = m_GameObjects[i]->GetModel()->GetNodes();

                    for (uint32_t nodeIndex = 0; nodeIndex < con.count; nodeIndex++)
                    {
                        ModelNodes& node = con.data[nodeIndex];

                        char subnodename[255];
                        itoa(nodeIndex, subnodename, 10);
                        strcat(subnodename, " subitem");
                        strcat(subnodename, name);
                        if (ImGui::CollapsingHeader(subnodename))
                        {
                            ImGui::PushID(i * nodeIndex);
                            if (
                                ImGui::InputFloat3("subTransform", node.position.GetXYZ()) ||
                                ImGui::InputFloat4("subRotation", node.rotation.GetXYZ()) ||
                                ImGui::InputFloat3("subScale", node.scale.GetXYZ())
                                ) 
                            {
                                node.transformBuf->UpdateTransform(node.position, node.rotation, node.scale);
                            }
                            ImGui::PopID();
                        }
                    }
                }
            }
        }

        ImGui::End();
    }

    void BBEngine::CalculateLightShadowMap(std::vector<GameObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, uint32_t a_Index)
    {
        Vector3 focusPoint = m_SpotLights[a_Index].position + m_SpotLights[a_Index].direction;
        DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(m_SpotLights[a_Index].position.x, m_SpotLights[a_Index].position.y, m_SpotLights[a_Index].position.z, 0),
            DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );

        m_Cam2.m_ViewMatrix = lightView;
        m_Graphics.SetCamera(&m_Cam2);
        m_Graphics.SetDepthStencilTarget(m_SLTextureDepthStencilViews[a_Index]);

        for (size_t i = 0; i < a_GameObjects.size(); i++)
        {
            GameObject* obj = a_GameObjects[i];
            obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
            obj->Draw(m_Graphics);
            obj->GetModel()->ResetShaders();
        }

        m_Graphics.ResetRenderTarget();
        m_Graphics.SetCamera(&m_Cam1);
    }

    void BBEngine::CalculateLightShadowMapSpotLight(std::vector<GameObject*>& a_GameObjects, uint32_t a_VSShadowMapShader, uint32_t a_PSShadowMapShader, PointLight a_Spotlight, uint32_t a_Index)
    {
        Vector3 viewDirections[6] = { 
            { 1.0f, 0.0f, 0.0f },    // Right
            { -1.0f, 0.0f, 0.0f },   // Left
            { 0.0f, 1.0f, 0.001f },   // top  
            { 0.0f, -1.0f, 0.001f },   // Bottom
            { 0.0f, 0.0f, 1.0f },   // front
            { 0.0f, 0.0f, -1.0f },   // back
        };

        m_Graphics.SetCamera(&m_Cam2);
        
        m_ShadowMapCBBuffer.index = a_Index;
        m_ShadowMapCB.Update(m_Graphics, m_ShadowMapCBBuffer);

        for (uint32_t depthStencilIndex = 0; depthStencilIndex < CUBEMAP_SIZE; depthStencilIndex++)
        {
            Vector3 focusPoint = a_Spotlight.position + viewDirections[depthStencilIndex];
            DirectX::XMMATRIX lightView = DirectX::XMMatrixLookAtLH(
                DirectX::XMVectorSet(a_Spotlight.position.x, a_Spotlight.position.y, a_Spotlight.position.z, 0),
                DirectX::XMVectorSet(focusPoint.x, focusPoint.y, focusPoint.z, 1.0f),
                DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f)
            );

            m_Graphics.SetDepthStencilTarget(/*a_Spotlight.*/m_PLTextureDepthStencilViews[a_Index][depthStencilIndex]);
            m_Cam2.m_ViewMatrix = lightView;

            for (size_t gameObjIndex = 0; gameObjIndex < a_GameObjects.size(); gameObjIndex++)
            {
                GameObject* obj = a_GameObjects[gameObjIndex];
                obj->GetModel()->SetCurrentShader(a_VSShadowMapShader, a_PSShadowMapShader);
                obj->Draw(m_Graphics);
                obj->GetModel()->ResetShaders();
            }

        }

        m_Graphics.ResetRenderTarget();
        m_Graphics.SetCamera(&m_Cam1);
    }

    void BBEngine::CheckInput()
    {
        Camera* cam = m_Graphics.GetCamera();
        Keyboard& kBoard = m_Window.m_Keyboard;

        //while (!kBoard.KeyIsEmpty())
        //{
        //    Keyboard::Event kEvent = kBoard.ReadKey();
        //    switch (kEvent.GetCode())
        //    {
        //    case 'W':
        //        cam->camForwardMove += 0.1f;
        //        break;
        //    case 'S':
        //        cam->camForwardMove += -0.1f;
        //        break;
        //    case 'A':
        //        cam->camRightMove += -0.1f;
        //        break;
        //    case 'D':
        //        cam->camRightMove += 0.1f;
        //        break;
        //    case VK_NUMPAD1:
        //        cam->camUpMove = 0.1f;
        //        break;
        //    case VK_NUMPAD0:
        //        cam->camUpMove = -0.1f;
        //        break;
        //    case VK_LEFT:
        //        cam->camYaw += -0.025f;
        //        break;
        //    case VK_RIGHT:
        //        cam->camYaw += 0.025f;
        //        break;
        //    case VK_UP:
        //        cam->camPitch += -0.025f;
        //        break;
        //    case VK_DOWN:
        //        cam->camPitch += 0.025f;
        //        break;
        //    case '1':
        //         m_Graphics.SetCamera(&m_Cam1);
        //        break;
        //    case '2':
        //         m_Graphics.SetCamera(&m_Cam2);
        //         break;
        //    default:
        //        break;
        //    }
        //}


        if (kBoard.KeyIsPressed('W'))
        {
            cam->camForwardMove += 0.1f;
        }
        if (kBoard.KeyIsPressed('S'))
        {
            cam->camForwardMove += -0.1f;
        }
        if (kBoard.KeyIsPressed('A'))
        {
            cam->camRightMove += -0.1f;
        }
        if (kBoard.KeyIsPressed('D'))
        {
            cam->camRightMove += 0.1f;
        }
        if (kBoard.KeyIsPressed(VK_NUMPAD1))
        {
            cam->camUpMove = 0.1f;
        }
        if (kBoard.KeyIsPressed(VK_NUMPAD0))
        {
            cam->camUpMove = -0.1f;
        }
        if (kBoard.KeyIsPressed(VK_LEFT))
        {
            cam->camYaw += -0.025f;
        }
        if (kBoard.KeyIsPressed(VK_RIGHT))
        {
            cam->camYaw += 0.025f;
        }
        if (kBoard.KeyIsPressed(VK_UP))
        {
            cam->camPitch += -0.025f;
        }
        if (kBoard.KeyIsPressed(VK_DOWN))
        {
            cam->camPitch += 0.025f;
        }

        if (kBoard.KeyIsPressed('1')) {
            m_Graphics.SetCamera(&m_Cam1);
        }

        if (kBoard.KeyIsPressed('2')) {
            m_Graphics.SetCamera(&m_Cam2);
        }

        cam->Update();
    }

    void BBEngine::RenderToTexture() {
        ID3D11Texture2D* texture;
        D3D11_TEXTURE2D_DESC tex_desc;
        tex_desc.Format = DXGI_FORMAT_A8_UNORM;
        tex_desc.Width = 1024;
        tex_desc.Height = 1024;
        tex_desc.MipLevels = 1;
        tex_desc.ArraySize = 1;
        tex_desc.SampleDesc.Count = 1;
        tex_desc.SampleDesc.Quality = 0;
        tex_desc.Usage = D3D11_USAGE_DEFAULT;
        tex_desc.BindFlags = D3D11_BIND_RENDER_TARGET;
        tex_desc.CPUAccessFlags = 0;
        tex_desc.MiscFlags = 0;

        m_Graphics.GetDevice()->CreateTexture2D(&tex_desc, nullptr, &texture);

        ID3D11RenderTargetView* renderTarget;
        D3D11_RENDER_TARGET_VIEW_DESC render_desc = {};
        render_desc.Format = DXGI_FORMAT_A8_UNORM;
        render_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        render_desc.Texture2D.MipSlice = 0;

        m_Graphics.GetDevice()->CreateRenderTargetView(texture, &render_desc, &renderTarget);

        m_Graphics.GetContext()->OMSetRenderTargets(1, &renderTarget, nullptr);

        const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
        m_Graphics.GetContext()->ClearRenderTargetView(renderTarget, color);

        m_Graphics.ResetRenderTarget();
    }

    void BBEngine::RenderDebugOptions()
    {
        //ImGui::ShowDemoWindow(&show_demo_window);0
        
        if (ImGui::Begin("Game Options"))
        {
            static int curVShader = 0;
            std::vector<const char*> vShaders;
            VertexShader* vShaderArray = m_Graphics.GetVertexShaderArray();
            for (uint32_t i = 0; i < 100; i++)
            {
                if (vShaderArray[i].m_Path.empty()) break;
                vShaders.push_back(vShaderArray[i].m_Path.c_str());
            }
            
            ImGui::Combo("VertexShaders", &curVShader, vShaders.data(), vShaders.size());
            if (ImGui::Button("Reload Vertex Shader"))
            {
                m_Graphics.ReloadShader(ShaderType::VertexShader, curVShader);
            }

            std::vector<const char*> pShaders;
            PixelShader* pShadersArray = m_Graphics.GetPixelShaderArray();
            for (uint32_t i = 0; i < 100; i++)
            {
                if (pShadersArray[i].m_Path.empty()) break;
                pShaders.push_back(pShadersArray[i].m_Path.c_str());
            }

            static int curPShader = 0;
            ImGui::Combo("PixelShaders", &curPShader, pShaders.data(), pShaders.size());
            if (ImGui::Button("Reload Pixel Shader"))
            {
                m_Graphics.ReloadShader(ShaderType::PixelShader, curPShader);
            }
        }
        ImGui::End();
    }
}