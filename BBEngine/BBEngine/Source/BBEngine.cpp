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

        //m_PointLights.Push_Back(PointLight(
        //    Vector3(0.0f, 2.0f, 0.0f),
        //    Vector3(0.0f, 0.2f, 0.0f),
        //    Vector4(0.0f, 0.0f, 0.0f, 1.0f),
        //    Vector4(1.0f, 1.0f, 1.0f, 1.0f),
        //    15.0f
        //));

        m_SpotLights.Push_Back(SpotLight(
            Vector3(8.0f, 2.0f, 0.0f),
            Vector3(-1.0f, 0.0f, 0.0f),
            5.f,
            Vector3(0.4f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            1000.0f
        ));
        
        m_PerFrameBuffer = PixelConstantBuffer<cbPerFrame>(m_Graphics);
        m_PerFrameBuffer.Bind(m_Graphics);

        uint32_t m_VertexShader = m_Graphics.CreateShader(ShaderType::VertexShader, L"Assets/VertexShader.hlsl");
        uint32_t m_PixelShader = m_Graphics.CreateShader(ShaderType::PixelShader, L"Assets/PixelShader.hlsl");

        m_RTTVertexShader = VertexShader(m_Graphics, L"Assets/VSDrawToTexture.hlsl");
        m_RTTPixelShader = PixelShader(m_Graphics, L"Assets/PSDrawToTexture.hlsl");

        m_VSShadowMapShader = VertexShader(m_Graphics, L"Assets/VSShadowMap.hlsl");
        m_PSShadowMapShader = PixelShader(m_Graphics, L"Assets/PSShadowMap.hlsl");

        Model* Sponza = BBNew(m_StackAllocator, Model)(m_Graphics, &m_SponzaFile, &m_VertexShader, &m_PixelShader);
        m_Models.push_back(Sponza);
        Model* lantern = BBNew(m_StackAllocator, Model)(m_Graphics, &m_LanternFile, &m_VertexShader, &m_PixelShader);
        m_Models.push_back(lantern);
        Model* car = BBNew(m_StackAllocator, Model)(m_Graphics, &m_CarFile, &m_VertexShader, &m_PixelShader);
        m_Models.push_back(car);
        Model* aBeautifulGame = BBNew(m_StackAllocator, Model)(m_Graphics, &m_ABeautifulGameFile, &m_VertexShader, &m_PixelShader);
        m_Models.push_back(aBeautifulGame);

        int XSize = 2, YSize = 2;
        for (size_t i = 0; i < XSize; i++) {
            for (size_t y = 0; y < YSize; y++) {
                GameObject* sponzaObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, Sponza, Vector3(i * 50, 0, y * 50));
                m_GameObjects.push_back(sponzaObj);
            }
        }

        GameObject* lanternObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, lantern, Vector3(3, 0, 0), Vector3(0, 0, 0), Vector3(0.2f, 0.2f, 0.2f));
        m_GameObjects.push_back(lanternObj);

        GameObject* carObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, car, Vector3(0, 2, 0), Vector3(0, 0, 0), Vector3(10, 10, 10));
        m_GameObjects.push_back(carObj);

        GameObject* beautifulGameObj = BBNew(m_StackAllocator, GameObject)(m_Graphics, aBeautifulGame, Vector3(0, 2, -25), Vector3(0, 0, 0), Vector3(10, 10, 10));
        m_GameObjects.push_back(beautifulGameObj);

        //Cameras
        m_Cam1.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Cam2.SetPosition(DirectX::XMVectorSet(0, 0, 0, 0));
        m_Graphics.SetCamera(&m_Cam1);
    }

    bool show_demo_window = true;
    float incr = 0;
    void BBEngine::Update()
    {
        m_Graphics.ClearBuffer(0.07f, 0.0f, 0.012f);
        
        CheckInput();

        cbPerFrame FrameConstantBuffer;
        FrameConstantBuffer.directionalLight = m_DirectionalLight;

        m_SpotLights[0].position.z = sin(incr += 0.01);

        for (uint32_t i = 0; i < m_SpotLights.Size(); i++) {
            FrameConstantBuffer.spotlights[i] = m_SpotLights[i];
        }
        
        for (uint32_t i = 0; i < m_PointLights.Size(); i++) {
            FrameConstantBuffer.pointlights[i] = m_PointLights[i];
        }

        m_PerFrameBuffer.Update(m_Graphics, FrameConstantBuffer);

        for (size_t i = 0; i < m_GameObjects.size(); i++)
        {
            m_GameObjects[i]->Update(m_Graphics);
        }

        m_Graphics.CalculateLightShadowMap();

        m_Graphics.BindDepthTexture();

        for (size_t i = 0; i < m_Models.size(); i++) {
            m_Models[i]->Draw(m_Graphics);
        }

        m_Graphics.UnbindSRV(1);

        ImGui::ShowDemoWindow(&show_demo_window);

        m_Graphics.EndFrame();
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
}