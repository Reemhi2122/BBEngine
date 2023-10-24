#include "keyboard.h"

bool Keyboard::KeyIsPressed(unsigned char a_Keycode) const noexcept
{
	return m_KeyStates[a_Keycode];
}

Keyboard::Event Keyboard::ReadKey() noexcept
{
	if (m_KeyBuffer.size() > 0u) {
		Keyboard::Event e = m_KeyBuffer.front();
		m_KeyBuffer.pop();
		return e;
	} else {
		return Keyboard::Event();
	}
}

bool Keyboard::KeyIsEmpty() const noexcept
{
	return m_KeyBuffer.empty();
}

void Keyboard::FlushKey() noexcept
{
	m_KeyBuffer = std::queue<Event>();
}

char Keyboard::ReadChar() noexcept
{
	if (m_CharBuffer.size() > 0u) {
		unsigned char charcode = m_CharBuffer.front();
		m_CharBuffer.pop();
		return charcode;
	} else {
		return 0;
	}
}

bool Keyboard::CharIsEmpty() const noexcept
{
	return m_CharBuffer.empty();
}

void Keyboard::FlushChar() noexcept
{
	m_CharBuffer = std::queue<char>();
}

void Keyboard::Flush() noexcept
{
	FlushKey();
	FlushChar();
}

void Keyboard::EnableAutorepeat() noexcept
{
	m_AutoRepeatEnabled = true;
}

void Keyboard::DisableAutorepeat() noexcept
{

	m_AutoRepeatEnabled = false;
}

bool Keyboard::AutorepeatIsEnabled() const noexcept
{
	return m_AutoRepeatEnabled;
}

void Keyboard::OnKeyPressed(unsigned char a_Keycode) noexcept
{
	m_KeyStates[a_Keycode] = true;
	m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::PRESS, a_Keycode));
	TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnKeyReleased(unsigned char a_Keycode) noexcept
{
	m_KeyStates[a_Keycode] = false;
	m_KeyBuffer.push(Keyboard::Event(Keyboard::Event::Type::RELEASE, a_Keycode));
	TrimBuffer(m_KeyBuffer);
}

void Keyboard::OnChar(char a_Character) noexcept
{
	m_CharBuffer.push(a_Character);
	TrimBuffer(m_CharBuffer);
}

void Keyboard::ClearState() noexcept
{
	m_KeyStates.reset();
}

template<typename T>
inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > m_BufferSize) {
		buffer.pop();
	}
}