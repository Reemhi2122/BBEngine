#pragma once
#include <queue>

class Mouse {
	friend class BBWindow;
public:
	class Event {
	public:
		enum class Type {
			LPRESS,
			LRELEASE,
			RPRESS,
			RRELEASE,
			WHEELUP,
			WHEELDOWN,
			MOVE,
			ENTER,
			LEAVE,
			INVALID
		};
	
	private:
		Type m_Type;
		bool m_LeftIsPressed;
		bool m_RightIsPressed;
		int m_X;
		int m_Y;

	public:
		Event() noexcept
			: m_Type(Type::INVALID), m_LeftIsPressed(false), m_RightIsPressed(false),
			m_X(0), m_Y(0) {}
		
		Event(Type a_Type, const Mouse& parent) noexcept
			: m_Type(a_Type), m_LeftIsPressed(parent.m_LeftIsPressed), m_RightIsPressed(parent.m_RightIsPressed),
			m_X(parent.m_X), m_Y(parent.m_Y) {}
		
		bool IsValid() const noexcept {
			return m_Type != Type::INVALID;
		}

		Type GetType() const noexcept {
			return m_Type;
		}

		std::pair<int, int> GetPos() const noexcept {
			return { m_X, m_Y };
		}

		int GetPosX() const noexcept {
			return m_X;
		}

		int GetPosY() const noexcept {
			return m_Y;
		}

		bool LeftIsPressed() const noexcept {
			return m_LeftIsPressed;
		}		
		
		bool RightIsPressed() const noexcept {
			return m_RightIsPressed;
		}
	};

public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept;
	void Flush() noexcept;

private:
	void OnMouseMove(int a_X, int a_Y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int a_X, int a_Y) noexcept;
	void OnLeftReleased(int a_X, int a_Y) noexcept;
	void OnRightPressed(int a_X, int a_Y) noexcept;
	void OnRightReleased(int a_X, int a_Y) noexcept;
	void OnWheelUp(int a_X, int a_Y) noexcept;
	void OnWheelDown(int a_X, int a_Y) noexcept;
	void OnWheelDelta(int a_X, int a_Y, int a_Delta) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int m_BufferSize = 16u;
	int m_X, m_Y;
	bool m_IsInWindow = false;
	bool m_LeftIsPressed = false;
	bool m_RightIsPressed = false;
	int m_WheelDeltaCarry = 0;
	std::queue<Event> m_Buffer;
};

inline bool Mouse::IsEmpty() const noexcept
{
	return m_Buffer.empty();
}