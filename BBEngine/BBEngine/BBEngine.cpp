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
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(10, 2);
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
        const float Gausian3x3Kernal[9]{
            1.f, 2.f, 1.f,
            2.f, 4.f, 2.f,
            1.f, 2.f, 1.f
        };

        //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

        //BMP bmp("C:/Users/svogels/Pictures/BMP/f1.bmp");
        //
        //Utility::ConvolutionDesc desc;
        //desc.buffer = bmp.GetBuffer();
        //desc.width = bmp.GetWidth();
        //desc.height = bmp.GetHeight();
        //desc.channelCount = 3;
        //desc.kernel = gaussian_blur;

        //Utility::ConvolutionMultiThreaded(desc, m_ThreadPool, 8);
        //
        //bmp.WriteBMP("blurImage.bmp");

        //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[us]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;

        //begin = std::chrono::steady_clock::now();

        BMP bmp1("C:/Users/svogels/Pictures/BMP/sky.bmp");

        //Utility::ConvolutionDesc desc1;
        //desc1.buffer = bmp1.GetBuffer();
        //desc1.width = bmp1.GetWidth();
        //desc1.height = bmp1.GetHeight();
        //desc1.channelCount = 3;
        //desc1.kernel = gaussian_blur;

        //Utility::Convolution(desc1);

        bmp1.WriteBMP("blurImage1.bmp");
   
        //end = std::chrono::steady_clock::now();
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[us]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
        //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds> (end - begin).count() << "[ms]" << std::endl;
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

//void ThreadTest(void*) {
//    Sleep(10 * 1000);
//
//    printf("done!");
//}
//
//void StaticThreadTest(void*) {
//    Sleep(1 * 1000);
//
//    printf("Static thread called!");
//}

//m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(6, 2);
//testdesc = reinterpret_cast<TaskDesc*>(m_ThreadPool->AddTask(ThreadTest));
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//m_ThreadPool->AddTask(ThreadTest);
//
//BBThreadHandle static_handle = m_ThreadPool->CreateStaticThread(StaticThreadTest);
//m_ThreadPool->DestoryStaticThread(static_handle);