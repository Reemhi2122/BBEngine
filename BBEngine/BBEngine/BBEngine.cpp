#pragma once
#include "BBEngine.h"

BBEngine::BBEngine()
	: m_Window(800, 300, "BBWindow test") {}

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
    const float t = m_Timer.Peek();
    std::ostringstream oss;
    oss << "Time elapsed: " << t;
    m_Window.SetTitle(oss.str());
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