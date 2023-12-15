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
        file = parser.Parse("Assets/Models/Cube/glTF/", "Cube.gltf");


        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> adist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> ddist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> odist(0.0f, 3.1415926f * 0.3f);
        std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

        m_Window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

        m_Model = BBNew(m_StackAllocator, Model)(m_Window.GetGraphics(), reinterpret_cast<Vertex*>(file->meshes[0].vertices), file->meshes[0].indices);

        for (size_t i = 0; i < 80; i++)
        {
            m_Boxes.push_back(BBNew(m_StackAllocator, Box)(m_Window.GetGraphics(), rng, adist, ddist, odist, rdist));
        }

    }

    bool show_demo_window = true;
    void BBEngine::Update()
    {
        float time = m_Timer.Stamp();
        m_Window.GetGraphics().ClearBuffer(0.07f, 0.0f, 0.012f);

        //for (size_t i = 0; i < m_Boxes.size(); i++) {
        //    m_Boxes[i]->Update(time);
        //    m_Boxes[i]->Draw(m_Window.GetGraphics());
        //}

        m_Model->Update(time);
        m_Model->Draw(m_Window.GetGraphics());

        ImGui::ShowDemoWindow(&show_demo_window);

        m_Window.GetGraphics().EndFrame();
    }
}