#pragma once
#include "BBEngine.h"

BBEngine::BBEngine()
	: m_Window(800, 600, "BBWindow test") 
{
}

int BBEngine::StartBBEngine()
{
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
    m_Window.GetGraphics().DrawTestTriangle();
    m_Window.GetGraphics().EndFrame();
}

//while (!m_Window.m_Mouse.IsEmpty()) {
//    const auto e = m_Window.m_Mouse.Read();
//    switch (e.GetType())
//    {
//    case Mouse::Event::Type::LEAVE:
//        m_Window.SetTitle("Gone!");
//        break;
//    case Mouse::Event::Type::MOVE:
//        std::ostringstream oss;
//        oss << "Mouse Moved (" << e.GetPosX() << "," << e.GetPosY() << ")";
//        m_Window.SetTitle(oss.str());
//        break;
//    }
//}