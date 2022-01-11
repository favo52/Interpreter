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
		// Store user's variable and check success
		auto inserted = m_VariableMap.insert(std::make_pair(key, value));
		assert(inserted.second);
	}

	template<typename Key, typename Value>
	inline bool VariableHolder<Key, Value>::Find(const Key& key)
	{
		auto x = m_VariableMap.find(key);
		if (x != m_VariableMap.end())
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
		auto found = m_VariableMap.find(key);
		assert(found != m_VariableMap.end());

		return found->second;
	}

	template<typename Key, typename Value>
	const Value& VariableHolder<typename Key, typename Value>::GetValue(Key key) const
	{
		auto found = m_VariableMap.find(key);
		assert(found != m_VariableMap.end());

		return found->second;
	}

	typedef VariableHolder<std::string, int> IntegerHolder;
	typedef VariableHolder<std::string, double> RealHolder;
	typedef VariableHolder<std::string, std::string> StringHolder;
}
