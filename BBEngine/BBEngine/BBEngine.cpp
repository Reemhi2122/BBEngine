#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "BBMemory.h"
#include "Allocators/ArenaAllocator.h"
#include "Allocators/StackAllocator.h"
#include "Allocators/PoolAllocator.h"
#include "Allocators/FreeListAllocator.h"
#include "Allocators/BuddyAllocator.h"

#include <chrono>
#include <iostream>
#include <cstdint>


BBEngine::BBEngine()
	: m_Window(800, 600, "BBWindow test") 
{
}

int BBEngine::StartBBEngine()
{
    BB_Log_Init("BBLogger", LOG_ALL, "./Test");

    TestCode();

    while (true) {
        if (const auto ecode = BBWindow::ProcessMessages())
            return *ecode;

        Update();
    }
}

void BBEngine::TestCode()
{
    BBE::Allocators::ArenaAllocator alloc;
    alloc.Init(1024);

    int* x = reinterpret_cast<int*>(alloc.Alloc(1024));
    int* y = reinterpret_cast<int*>(alloc.Alloc(1024));
    int* z = reinterpret_cast<int*>(alloc.Alloc(1024));
    int* w = reinterpret_cast<int*>(alloc.Alloc(1024));
    int* a = reinterpret_cast<int*>(alloc.Alloc(1024));

    *x = 4;
    *y = 8;
    *z = 16;
    *w = 32;
    *a = 64;


    BB_LogF(0, BBUtility::LogInfo, "values x: %d - y: %d - z: %d - w: %d - a: %d", *x, *y, *z, *w, *a);
}

void BBEngine::Update()
{
    const float c = std::sin(m_Timer.Peek()) / 2.0f + 0.5f;
    m_Window.GetGraphics().ClearBuffer(c,c, 1.0f);

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