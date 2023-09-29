#pragma once

template<typename T>
class Pool {
public:
	Pool() = default;
	~Pool() = default;

	T* Pop();
	T* Add();

private:
	void* m_Head;

};

