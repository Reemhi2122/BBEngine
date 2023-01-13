#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	friend class BBWindow;

public:
	class Event {
	public:
		enum class Type {
			PRESS,
			RELEASE,
			INVALID
		};
	private:
		Type m_Type;
		unsigned char m_Code;
	public:
		Event() : m_Type(Type::INVALID), m_Code(0u) {}
		Event(Type a_Type, unsigned char a_Code) : m_Type(a_Type), m_Code(a_Code) {}
		
		bool IsPressed() const noexcept {
			return m_Type == Type::PRESS;
		}

		bool IsReleased() {
			return m_Type == Type::RELEASE;
		}

		bool IsValid() {
			return m_Type != Type::INVALID;
		}

		unsigned char GetCode() const noexcept {
			return m_Code;
		}
	};

	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;

	bool KeyIsPressed(unsigned char a_Keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void FlushKey() noexcept;

	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;

	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;

private:
	void OnKeyPressed(unsigned char a_Keycode) noexcept;
	void OnKeyReleased(unsigned char a_Keycode) noexcept;
	void OnChar(char a_Character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int m_NKeys = 256u;
	static constexpr unsigned int m_BufferSize = 16u;
	bool m_AutoRepeatEnabled = false;
	std::bitset<m_NKeys> m_KeyStates;
	std::queue<Event> m_KeyBuffer;
	std::queue<char> m_CharBuffer;
};
