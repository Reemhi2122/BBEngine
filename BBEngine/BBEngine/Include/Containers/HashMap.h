#pragma once
#include "Utility/BBMemory.h"
#include "Containers/LinkedList.h"

namespace BBE {

	constexpr uint32_t DEFAULT_SIZE = 255;

	template<typename Key, typename Value>
	class HashMap
	{
	public:
		HashMap();
		HashMap(uint32_t a_HashMapArraySize);
		~HashMap();

		int Insert(Key a_Key, Value a_Value);
		Value Get(Key a_Key);

	private:
		uint32_t StringToHash(const char* a_Str);

		LinkedList<Value>* m_HashMap;
		uint32_t m_MapSize;
		Allocators::ArenaAllocator M_HashMapAlloc;
	};

	template<typename Key, typename Value>
	HashMap<Key, Value>::HashMap() {
		M_HashMapAlloc.Init(DEFAULT_SIZE * sizeof(LinkedList<Value>));
		m_HashMap = BBNewArr(M_HashMapAlloc, DEFAULT_SIZE, LinkedList<Value>);
		m_MapSize = DEFAULT_SIZE;
	}

	template<typename Key, typename Value>
	HashMap<Key, Value>::HashMap(uint32_t a_HashMapArraySize) {
		M_HashMapAlloc.Init(a_HashMapArraySize * sizeof(LinkedList<Value>));
		m_HashMap = BBNewArr(M_HashMapAlloc, a_HashMapArraySize, LinkedList<Value>);
		m_MapSize = a_HashMapArraySize;
	}

	template<typename Key, typename Value>
	HashMap<Key, Value>::~HashMap() {
		BBFreeArr(M_HashMapAlloc, m_HashMap);
		M_HashMapAlloc.Clear();
	}

	template<typename Key, typename Value>
	int HashMap<Key, Value>::Insert(Key a_Key, Value a_Value) {
		uint32_t index = StringToHash(static_cast<std::string>(a_Key).c_str());
		uint32_t mappedIndex = index % m_MapSize;
		m_HashMap[mappedIndex].Push_Back(a_Value);
		return index;
	}

	template<typename Key, typename Value>
	Value HashMap<Key, Value>::Get(Key a_Key) {
		uint32_t index = StringToHash(static_cast<std::string>(a_Key).c_str());
		uint32_t mappedIndex = index % m_MapSize;
		return m_HashMap[mappedIndex][0];
	}

#define A 54059 /* a prime */
#define B 76963 /* another prime */
#define C 86969 /* yet another prime */
#define FIRSTH 37 /* also prime */

	template<typename Key, typename Value>
	uint32_t HashMap<Key, Value>::StringToHash(const char* a_Str) {
		unsigned h = FIRSTH;
		while (*a_Str) {
			h = (h * A) ^ (a_Str[0] * B);
			a_Str++;
		}
		return h;
	}
}