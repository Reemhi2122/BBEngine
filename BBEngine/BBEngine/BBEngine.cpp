#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"

#include <chrono>
#include <iostream>
#include <cstdint>

#pragma region TempIncludes
#include "FileLoaders/Image/BMPLoader.h"
#include "Utility/ImageProcessing.h"
#pragma endregion //Temp Includes

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

namespace BBE {

    TaskDesc* testdesc = nullptr;

    BBEngine::BBEngine()
        : m_Window(800, 600, "BBWindow test")
    {
        BB_Log_Init("BBLogger", LOG_ALL, "logs/");
        m_ArenaAllocator.Init(BBE::PageSize);
        m_StackAllocator.Init(128 * BBE::MBSize);
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(8, 2);

        std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> adist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> ddist(0.0f, 3.1415926f * 2.0f);
        std::uniform_real_distribution<float> odist(0.0f, 3.1415926f * 0.3f);
        std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

        m_Window.GetGraphics().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));

        for (size_t i = 0; i < 80; i++) {
            m_Boxes.push_back(BBNew(m_StackAllocator, Box)(m_Window.GetGraphics(), rng, adist, ddist, odist, rdist));
        }
    }

    BBEngine::~BBEngine()
    {
        BBFree(m_ArenaAllocator, m_ThreadPool);
    }

    int BBEngine::StartBBEngine()
    {
        TestCode();

        try {
            while (true) {
                if (const auto ecode = BBWindow::ProcessMessages())
                    return *ecode;

                Update();
            }
        }
        catch (const BBException& e) {
            MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
        }
        catch (const std::exception& e) {
            MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        catch (...) {
            MessageBox(nullptr, "No details available", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
        }
        return -1;

    }

    void BBEngine::TestCode()
    {
    }

    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    void BBEngine::Update()
    {
        float time = m_Timer.Stamp();
        m_Window.GetGraphics().ClearBuffer(0.07f, 0.0f, 0.012f);

        for (size_t i = 0; i < m_Boxes.size(); i++) {
            m_Boxes[i]->Update(time);
            m_Boxes[i]->Draw(m_Window.GetGraphics());
        }

        //m_Window.GetGraphics().DrawTestTriangle(90, 0, 0);

        ImGui::ShowDemoWindow(&show_demo_window);

        m_Window.GetGraphics().EndFrame();
    }
}

/*std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
BMP bmp("assets/Images/Lenna.bmp");

Utility::ConvolutionDesc desc;
desc.buffer = bmp.GetBuffer();
desc.height = bmp.GetHeight();
desc.width = bmp.GetWidth();
desc.channelCount = 3;
desc.kernel = gaussian_blur;

for (int i = 0; i < 255; i++) {
    Utility::Convolution(desc, m_StackAllocator);
}

bmp.WriteBMP("LennaBlur.bmp");

std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
std::cout << "No SIMD" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[us]" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;

std::chrono::steady_clock::time_point begin2 = std::chrono::steady_clock::now();

BMP bmp2("assets/Images/Lenna.bmp");

Utility::ConvolutionDesc desc2;
desc2.buffer = bmp2.GetBuffer();
desc2.height = bmp2.GetHeight();
desc2.width = bmp2.GetWidth();
desc2.channelCount = 3;
desc2.kernel = gaussian_blur;

for (int i = 0; i < 255; i++) {
    Utility::ConvolutionSIMD(desc2, m_StackAllocator);
}

bmp2.WriteBMP("LennaBlur2.bmp");

std::chrono::steady_clock::time_point end2 = std::chrono::steady_clock::now();
std::cout << "SIMD" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2).count() << "[us]" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end2 - begin2).count() << "[ns]" << std::endl;
std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end2 - begin2).count() << "[ms]" << std::endl;*/