#pragma once
#include "Utility/BBMemory.h"

namespace BBE {

	constexpr uint32_t DefaultSize = 255;

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
		uint32_t StringToHash();

		Value* m_HashMap
	};

	template<typename Key, typename Value>
	HashMap<Key, Value>::HashMap() {

	}

	template<typename Key, typename Value>
	HashMap<Key, Value>::HashMap(uint32_t a_HashMapArraySize) {

	}

	template<typename Key, typename Value>
	HashMap<Key, Value>::~HashMap() {
	
	}

	template<typename Key, typename Value>
	int HashMap<Key, Value>::Insert(Key a_Key, Value a_Value) {

	}

	template<typename Key, typename Value>
	Value HashMap<Key, Value>::Get(Key a_Key) {

	}

	template<typename Key, typename Value>
	uint32_t HashMap<Key, Value>::StringToHash() {

	}
}