#pragma once
#include "BBEngine.h"
#include "Matrix4x4.h"
#include "Logger/Logger.h"

BBEngine::BBEngine()
	: m_Window(800, 600, "BBWindow test") 
{
}

int BBEngine::StartBBEngine()
{
    BBlogger::Logger* testLogger = BBlogger::Logger::GetInstance();
    testLogger->SetupLogger("BBLogger");

    testLogger->Log(BBlogger::LogFlag::LogInfo,             "Testing the log funtion - info");
    testLogger->Log(BBlogger::LogFlag::LogWarningLow,       "Testing the log funtion - warning");
    testLogger->Log(BBlogger::LogFlag::LogWarningMedium,    "Testing the log funtion - error");
    testLogger->Log(BBlogger::LogFlag::LogWarningHigh,      "Testing the log funtion - error");
    testLogger->Log(BBlogger::LogFlag::LogAssert,           "Testing the log funtion - error");

    testLogger->LogF(BBlogger::LogFlag::LogAssert,          "Testing the logf funtion with int: %d, float: %f, and char: %c - Error", 23, 1.34f, 'd');

    while (true) {
        if (const auto ecode = BBWindow::ProcessMessages())
            return *ecode;

        Update();
    }
}

void BBEngine::Update()
{
    const float c = std::sin(m_Timer.Peek()) / 2.0f + 0.5f;
    m_Window.GetGraphics().ClearBuffer(c,c, 1.0f);
    m_Window.GetGraphics().DrawTestTriangle(
        m_Timer.Peek(), 
        m_Window.m_Mouse.GetPosX() / 400.0f - 1.0f, 
        -m_Window.m_Mouse.GetPosY() / 300.0f + 1.0f);
    m_Window.GetGraphics().EndFrame();
}