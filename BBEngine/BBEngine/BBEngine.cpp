#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "Allocators/Allocator.h"
#include "Allocators/ArenaAllocator.h"
#include "Allocators/StackAllocator.h"
#include "Allocators/PoolAllocator.h"
#include "Allocators/FreeListAllocator.h"
#include "Allocators/BuddyAllocator.h"

#include "Utility/BBMemory.h"

#include "Thread/ThreadPool.h"

#include <chrono>
#include <iostream>
#include <cstdint>

namespace BBE {

    BBEngine::BBEngine()
        : m_Window(800, 600, "BBWindow test")
    {
        m_ThreadPool = nullptr;
    }

    BBEngine::~BBEngine()
    {
        BBFree(m_ArenaAllocator, m_ThreadPool);
    }

    int BBEngine::StartBBEngine()
    {
        BB_Log_Init("BBLogger", LOG_ALL, "logs/");
        m_ArenaAllocator.Init(BBE::PageSize);

        TestCode();

        while (true) {
            if (const auto ecode = BBWindow::ProcessMessages())
                return *ecode;

            Update();
        }
    }

    void ThreadTest(void*) {
        Sleep(10 * 1000);

        printf("done!");
    }

    void BBEngine::TestCode()
    {
        m_ThreadPool = BBNew(m_ArenaAllocator, BBE::ThreadPool)(6);
        m_ThreadPool->AddTask(ThreadTest);
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