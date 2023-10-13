#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Utility/BBMemory.h"
#include "Thread/ThreadPool.h"
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
        m_StackAllocator.Init(4 * BBE::MBSize);
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
        BMP bmp1("C:/Users/svogels/Pictures/BMP/BlurImage.bmp");

        Utility::ConvolutionDesc desc1;
        desc1.buffer = bmp1.GetBuffer();
        desc1.width = bmp1.GetWidth();
        desc1.height = bmp1.GetHeight();
        desc1.channelCount = 3;
        desc1.kernel = gaussian_blur;

        Utility::Convolution(desc1, m_StackAllocator);

        bmp1.WriteBMP("gaussianblur5x5.bmp");
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
