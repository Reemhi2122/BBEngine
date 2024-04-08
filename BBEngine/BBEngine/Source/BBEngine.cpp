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
        Start();

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

    void BBEngine::Start()
    {
        GLTFParser parser;
        parser.Parse("Assets/Models/Sponza/Sponza/", "Sponza.gltf", &m_SponzaFile);
        parser.Parse("Assets/Models/Lantern/glTF/", "Lantern.gltf", &m_LanternFile);
        parser.Parse("Assets/Models/ToyCar/glTF/", "ToyCar.gltf", &m_CarFile);
        parser.Parse("Assets/Models/Fox/glTF/", "Fox.gltf", &m_FoxFile);

        m_Graphics.SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 1000.0f));

        m_DirectionalLight = DirectionalLight(
            Vector3(0.25f, 0.5f, -1.0f), 
            Vector4(0.1f, 0.1f, 0.1f, 1.0f), 
            Vector4(0.5f, 0.5f, 0.5f, 1.0f)
        );

        m_SpotLight = SpotLight(
            Vector3(0.0f, 2.0f, 0.0f),
            Vector3(0.0f, 0.2f, 0.0f),
            Vector4(0.0f, 0.0f, 0.0f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            15.0f
        );
        
        m_PerFrameBuffer = PixelConstantBuffer<cbPerFrame>(m_Graphics);
        m_PerFrameBuffer.Bind(m_Graphics);

        m_VertexShader = VertexShader(m_Graphics, L"Assets/VertexShader.hlsl");
        m_PixelShader = PixelShader(m_Graphics, L"Assets/PixelShader.hlsl");

        m_RTTVertexShader = VertexShader(m_Graphics, L"Assets/VSDrawToTexture.hlsl");
        m_RTTPixelShader = PixelShader(m_Graphics, L"Assets/PSDrawToTexture.hlsl");

        Model* Sponza = BBNew(m_StackAllocator, Model)(m_Graphics, &m_SponzaFile, &m_VertexShader, &m_PixelShader);
        Model* lantern = BBNew(m_StackAllocator, Model)(m_Graphics, &m_LanternFile, &m_VertexShader, &m_PixelShader);
        Model* car = BBNew(m_StackAllocator, Model)(m_Graphics, &m_CarFile, &m_VertexShader, &m_PixelShader);

        int XSize = 2, YSize = 2;
        for (size_t i = 0; i < XSize; i++) {
            for (size_t y = 0; y < YSize; y++) {
                GameObject* obj = BBNew(m_StackAllocator, GameObject)(Sponza);
                //obj->SetPosition(Vector3(i * 50, 0, y * 50));
                m_Model.push_back(obj);
            }
        }

        //lantern->SetPosition(Vector3(3, 0, 0));
        //lantern->SetScale(Vector3(0.2f, 0.2f, 0.2f));
        //m_Model.push_back(lantern);

        //car->SetPosition(Vector3(0, 2, 0));
        //car->SetRotation(Vector3(3.1415f / 2.f, 0, 0));
        //car->SetScale(Vector3(10, 10, 10));
        //m_Model.push_back(car);

        //Model* fox = BBNew(m_StackAllocator, Model)(m_Graphics, &m_FoxFile, &m_VertexShader, &m_PixelShader);
        //fox->SetPosition(Vector3(-3, 2, 0));
        //m_Model.push_back(fox);
    }

    bool show_demo_window = true;
    void BBEngine::Update()
    {
        float time = m_Timer.Stamp();
        m_Graphics.ClearBuffer(0.07f, 0.0f, 0.012f);
        
        CheckInput();

        for (size_t i = 0; i < m_Model.size(); i++)
        {
            m_Model[i]->Update(m_Graphics);
        }

        //cbPerFrame test = { m_DirectionalLight, m_SpotLight};
        //m_PerFrameBuffer.Update(m_Graphics, test);

        //for (size_t i = 0; i < m_Model.size(); i++)
        //{
        //    m_Model[i]->Draw(m_Graphics);
        //}

        ImGui::ShowDemoWindow(&show_demo_window);
        m_Graphics.EndFrame();
    }

    void BBEngine::CheckInput()
    {
        Camera* cam = m_Graphics.GetCamera();

        if (m_Window.m_Keyboard.KeyIsPressed('W'))
        {
            cam->camForwardMove += 0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed('S'))
        {
            cam->camForwardMove += -0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed('A'))
        {
            cam->camRightMove += -0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed('D'))
        {
            cam->camRightMove += 0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_NUMPAD1))
        {
            cam->camUpMove = 0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_NUMPAD0))
        {
            cam->camUpMove = -0.1f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_LEFT))
        {
            cam->camYaw += -0.025f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_RIGHT))
        {
            cam->camYaw += 0.025f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_UP))
        {
            cam->camPitch += -0.025f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_DOWN))
        {
            cam->camPitch += 0.025f;
        }

        cam->Update();
    }

    void BBEngine::RenderToTexture() {
        m_DrawToTexture = BBNew(m_StackAllocator, DrawToTexture)(m_Graphics, &m_RTTVertexShader, &m_RTTPixelShader);

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


        for (size_t i = 0; i < m_Model.size(); i++)
        {
            m_Model[i]->Update(0.0f);
        }

        for (size_t i = 0; i < m_Model.size(); i++)
        {
            m_Model[i]->Draw(m_Graphics);
        }

        m_Graphics.ResetRenderTarget();
    }
}