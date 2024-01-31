#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"
#include "FileLoaders/Models/GLTFParser.h"

#include <chrono>
#include <iostream>
#include <cstdint>

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

#include <DirectXMath.h>
#include "Camera.h"

namespace BBE
{
    TaskDesc* testdesc = nullptr;
    GLTFFile* file;

    BBEngine::BBEngine()
        : m_Window(800, 600, "BBWindow test")
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
        TestCode();

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

    void BBEngine::TestCode()
    {
        GLTFParser parser;
        //file = parser.Parse("Assets/Models/Lantern/glTF/", "Lantern.gltf");
        file = parser.Parse("Assets/Models/Sponza/Sponza/", "Sponza.gltf");

        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> adist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> ddist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> odist(0.0f, 3.1415926f * 0.3f);
        std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

        m_Window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 1000.0f));

        m_Model.push_back(BBNew(m_StackAllocator, Model)(m_Window.GetGraphics(), file->nodes[0], file));
        //m_Model.push_back(BBNew(m_StackAllocator, Model)(m_Window.GetGraphics(), file->nodes[1], file));
        //m_Model.push_back(BBNew(m_StackAllocator, Model)(m_Window.GetGraphics(), file->nodes[2], file));
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
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->forward, 0.5f)));
        }

        if (m_Window.m_Keyboard.KeyIsPressed('S'))
        {
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->forward, -0.5f)));
        }

        if (m_Window.m_Keyboard.KeyIsPressed('A'))
        {
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->right, -0.5f)));
        }

        if (m_Window.m_Keyboard.KeyIsPressed('D'))
        {
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->right, 0.5f)));
        }

        if (m_Window.m_Keyboard.KeyIsPressed(VK_DOWN))
        {
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->up, -0.5f)));
        }

        if (m_Window.m_Keyboard.KeyIsPressed(VK_UP))
        {
            cam->SetPosition(DirectX::XMVectorAdd(cam->GetPosition(), DirectX::XMVectorScale(cam->up, 0.5f)));
        }

        cam->SetViewMatrix(DirectX::XMMatrixLookToLH(cam->GetPosition(), cam->forward, cam->up) * m_Window.GetGraphics().GetProjection());
    }
}

//for (size_t i = 0; i < 80; i++)
//{
//    m_Boxes.push_back(BBNew(m_StackAllocator, Box)(m_Window.GetGraphics(), rng, adist, ddist, odist, rdist));
//}

//for (size_t i = 0; i < m_Boxes.size(); i++) {
//    m_Boxes[i]->Update(time);
//    m_Boxes[i]->Draw(m_Window.GetGraphics());
//}