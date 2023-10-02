#pragma once

namespace BBE {

	template<typename T>
	struct PoolElement {
		T element;
		PoolElement<T>* nextElement;
	};

	template<typename T>
	class Pool {
	public:
		Pool();
		~Pool();

		T* Pop();
		
		void PushBack(T* a_Element);

	private:
		void InitNewElements(size_t a_num = 5);

		PoolElement<T>* m_Head;
	};

	template<typename T>
	Pool<T>::Pool() {
		InitNewElements();
	}

	template<typename T>
	Pool<T>::~Pool() {
		printf("test");
	}

	template<typename T>
	void Pool<T>::InitNewElements(size_t a_num) {
		PoolElement<T>* prev = NULL;

		for (size_t i = 0; i < a_num; i++) {
			PoolElement<T> el;
			el.element = T();
			el.nextElement = prev;
			prev = &el;
		}
		
		m_Head = prev;
	}
}