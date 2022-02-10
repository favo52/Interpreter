#pragma once

#include "pch.h"

namespace Interpreter
{
	template<typename Key, typename Value>
	class VariableHolder
	{
	public:
		void InsertToMap(Key key, Value value);

		bool Find(const Key& key);
		void Clear();

		Value& GetValue(Key key);
		const Value& GetValue(Key key) const;

	private:
		std::unordered_map<Key, Value> m_VariableMap;
	};

	template<typename Key, typename Value>
	void VariableHolder<typename Key, typename Value>::InsertToMap(Key key, Value value)
	{
		// Replace if already exists, add new entry if not
		m_VariableMap[key] = value;
	}

	template<typename Key, typename Value>
	inline bool VariableHolder<Key, Value>::Find(const Key& key)
	{
		auto itr = m_VariableMap.find(key);
		if (itr != m_VariableMap.end())
			return true;

		return false;
	}

	template<typename Key, typename Value>
	inline void VariableHolder<Key, Value>::Clear()
	{
		m_VariableMap.clear();
	}

	template<typename Key, typename Value>
	Value& VariableHolder<typename Key, typename Value>::GetValue(Key key)
	{
		auto itr = m_VariableMap.find(key);
		assert(itr != m_VariableMap.end());

		return itr->second;
	}

	template<typename Key, typename Value>
	const Value& VariableHolder<typename Key, typename Value>::GetValue(Key key) const
	{
		auto itr = m_VariableMap.find(key);
		assert(itr != m_VariableMap.end());

		return itr->second;
	}

	typedef VariableHolder<std::string, int> IntegerHolder;
	typedef VariableHolder<std::string, double> RealHolder;
	typedef VariableHolder<std::string, std::string> StringHolder;
}
