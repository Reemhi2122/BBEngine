#include "mouse.h"
#include "windows.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return {m_X, m_Y};
}

int Mouse::GetPosX() const noexcept
{
	return m_X;
}

int Mouse::GetPosY() const noexcept
{
	return m_Y;
}

bool Mouse::IsInWindow() const noexcept
{
	return m_IsInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return m_LeftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return m_RightIsPressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if (m_Buffer.size() > 0u) {
		Mouse::Event e = m_Buffer.front();
		m_Buffer.pop();
		return e;
	} else {
		return Mouse::Event();
	}
}

void Mouse::Flush() noexcept
{
	m_Buffer = std::queue<Mouse::Event>();
}

void Mouse::OnMouseMove(int a_X, int a_Y) noexcept
{
	m_X = a_X;
	m_Y = a_Y;

	m_Buffer.push(Mouse::Event(Mouse::Event::Type::MOVE, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	m_IsInWindow = false;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LEAVE, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	m_IsInWindow = true;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::ENTER, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int a_X, int a_Y) noexcept
{
	m_LeftIsPressed = true;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LPRESS, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int a_X, int a_Y) noexcept
{
	m_LeftIsPressed = false;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRELEASE, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int a_X, int a_Y) noexcept
{
	m_RightIsPressed = true;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::RPRESS, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int a_X, int a_Y) noexcept
{
	m_RightIsPressed = false;
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::LRELEASE, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int a_X, int a_Y) noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WHEELUP, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int a_X, int a_Y) noexcept
{
	m_Buffer.push(Mouse::Event(Mouse::Event::Type::WHEELDOWN, *this));
	TrimBuffer();
}

void Mouse::OnWheelDelta(int a_X, int a_Y, int a_Delta) noexcept
{
	m_WheelDeltaCarry += a_Delta;
	while (m_WheelDeltaCarry >= WHEEL_DELTA) {
		m_WheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(a_Y, a_Y);
	}

	while (m_WheelDeltaCarry <= WHEEL_DELTA)
	{
		m_WheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(a_Y, a_Y);
	}
}

void Mouse::TrimBuffer() noexcept
{
	while(m_Buffer.size() > m_BufferSize) {
		m_Buffer.pop();
	}
}