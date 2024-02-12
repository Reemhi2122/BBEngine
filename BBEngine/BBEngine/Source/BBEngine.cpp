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

namespace BBE
{
    TaskDesc* testdesc = nullptr;
    GLTFFile* file;

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
        file = parser.Parse("Assets/Models/Sponza/Sponza/", "Sponza.gltf");

        m_Window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 1000.0f));

        m_DirectionalLight = DirectionalLight(Vector3(0.25f, 0.5f, -1.0f), Vector4(1.f, 1.f, 1.f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 0.0f);

        m_SpotLight = SpotLight(
            Vector3(0.0f, 0.0f, 0.0f),
            Vector3(0.0f, 0.2f, 0.0f),
            Vector4(0.3f, 0.3f, 0.3f, 1.0f),
            Vector4(1.0f, 1.0f, 1.0f, 1.0f),
            100.f
        );
        
        m_PerFrameBuffer = PixelConstantBuffer<cbPerFrame>(m_Window.GetGraphics());

        m_VertexShader = VertexShader(m_Window.GetGraphics(), L"Assets/VertexShader.hlsl");
        m_PixelShader = PixelShader(m_Window.GetGraphics(), L"Assets/PixelShader.hlsl");

        for (size_t nodeIndex = 0; nodeIndex < file->nodeAmount; nodeIndex++)
        {
            for (size_t primitiveIndex = 0; primitiveIndex < file->nodes[nodeIndex].mesh.primitiveCount; primitiveIndex++)
            {
                m_Model.push_back(BBNew(m_StackAllocator, Model)(m_Window.GetGraphics(), file->nodes[nodeIndex].mesh.primative[primitiveIndex], file, &m_VertexShader, &m_PixelShader));
            }
        }
    }

    bool show_demo_window = true;
    void BBEngine::Update()
    {
        float time = m_Timer.Stamp();
        m_Window.GetGraphics().ClearBuffer(0.07f, 0.0f, 0.012f);
        
        CheckInput();

        for (size_t i = 0; i < m_Model.size(); i++)
        {
            m_Model[i]->Update(time);
        }

        cbPerFrame test = { m_DirectionalLight };
        m_PerFrameBuffer.Update(m_Window.GetGraphics(), test);
        m_PerFrameBuffer.Bind(m_Window.GetGraphics());

        for (size_t i = 0; i < m_Model.size(); i++)
        {
            m_Model[i]->Draw(m_Window.GetGraphics());
        }

        ImGui::ShowDemoWindow(&show_demo_window);
        m_Window.GetGraphics().EndFrame();
    }

    void BBEngine::CheckInput()
    {
        Camera* cam = m_Window.GetGraphics().GetCamera();

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
            cam->camYaw += -0.01f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_RIGHT))
        {
            cam->camYaw += 0.01f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_UP))
        {
            cam->camPitch += -0.01f;
        }
        if (m_Window.m_Keyboard.KeyIsPressed(VK_DOWN))
        {
            cam->camPitch += 0.01f;
        }

        cam->Update();
    }
}