#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

#include "BBMemory.h"
#include "Allocators/ArenaAlloc.h"
#include "Allocators/StackAllocator.h"

#include <chrono>
#include <iostream>
#include <cstdint>


BBEngine::BBEngine()
	: m_Window(800, 600, "BBWindow test") 
{
}

int BBEngine::StartBBEngine()
{
    BB_Log_Init("BBLogger", BBUtility::LogFlag::LogInfo, "./Test");

    TestCode();

    while (true) {
        if (const auto ecode = BBWindow::ProcessMessages())
            return *ecode;

        Update();
    }
}

void BBEngine::TestCode()
{
    BBE::Allocators::StackAllocator alloc;
    alloc.Init(1024 * sizeof(int));

    alloc.Alloc(12);
    alloc.Alloc(4);


    BBMath::Matrix4x4 matrix
    {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    BBMath::Matrix4x4 matrix2
    {
        1.0f, 2.0f, 3.0f, 4.0f,
        5.0f, 6.0f, 7.0f, 8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };

    BBMath::Matrix4x4 matrix3 = matrix * matrix2;

    BB_LogF(0u, BBUtility::LogFlag::LogInfo,
        "%f - %f - %f - %f \n%f - %f - %f - %f \n%f - %f - %f - %f \n%f - %f - %f - %f",
        matrix3.m[0], matrix3.m[1], matrix3.m[2], matrix3.m[3],
        matrix3.m[4], matrix3.m[5], matrix3.m[6], matrix3.m[7],
        matrix3.m[8], matrix3.m[9], matrix3.m[10], matrix3.m[11],
        matrix3.m[12], matrix3.m[13], matrix3.m[14], matrix3.m[15]);

    BB_Log(0u, BBUtility::LogFlag::LogInfo, "test");

    BB_LogF(0u, BBUtility::LogFlag::LogInfo,
        "%f - %f - %f - %f \n%f - %f - %f - %f \n%f - %f - %f - %f \n%f - %f - %f - %f",
        matrix3.m[0], matrix3.m[1], matrix3.m[2], matrix3.m[3],
        matrix3.m[4], matrix3.m[5], matrix3.m[6], matrix3.m[7],
        matrix3.m[8], matrix3.m[9], matrix3.m[10], matrix3.m[11],
        matrix3.m[12], matrix3.m[13], matrix3.m[14], matrix3.m[15]);

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