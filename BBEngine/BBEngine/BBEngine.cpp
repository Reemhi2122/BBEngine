#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"

//temp
#include "FileLoaders/BMPLoader.h"
#include "Utility/ImageProcessing.h"

#include <chrono>
#include <iostream>
#include <cstdint>

namespace BBE {

    TaskDesc* testdesc = nullptr;

    BBEngine::BBEngine()
        : m_Window(800, 600, "BBWindow test")
    {
        BB_Log_Init("BBLogger", LOG_ALL, "logs/");
        m_ArenaAllocator.Init(BBE::PageSize);
        m_StackAllocator.Init(1 * BBE::MBSize);
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(16, 2);
    }

    BBEngine::~BBEngine()
    {
        BBFree(m_ArenaAllocator, m_ThreadPool);
    }

    int BBEngine::StartBBEngine()
    {
        TestCode();

        while (true) {
            if (const auto ecode = BBWindow::ProcessMessages())
                return *ecode;

            Update();
        }
    }

    void BBEngine::TestCode()
    {
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        BMP bmp("assets/Images/Lenna.bmp");
        
        Utility::ConvolutionDesc desc;
        desc.buffer = bmp.GetBuffer();
        desc.height = bmp.GetHeight();
        desc.width = bmp.GetWidth();
        desc.channelCount = 3;
        desc.kernel = gaussian_blur;

        Utility::Convolution(desc, m_StackAllocator);

        bmp.WriteBMP("LennaBlur.bmp");

        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[us]" << std::endl;
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
        std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
    }

    void BBEngine::Update()
    {
        const float c = std::sin(m_Timer.Peek()) / 2.0f + 0.5f;
        m_Window.GetGraphics().ClearBuffer(c, c, 1.0f);

        m_Window.GetGraphics().DrawTestTriangle(
            m_Timer.Peek(),
            0.0f,
            0.0f);

        m_Window.GetGraphics().DrawTestTriangle(
            m_Timer.Peek(),
            m_Window.m_Mouse.GetPosX() / 400.0f - 1.0f,
            -m_Window.m_Mouse.GetPosY() / 300.0f + 1.0f);
        m_Window.GetGraphics().EndFrame();
    }
}
