#pragma once

#include "pch.h"

namespace Interpreter
{
	template<typename Key, typename Value>
	class VariableHolder
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="key">The key to store.</param>
		/// <param name="value">The key's value.</param>
		/// <returns>True if insertion succeeded, false otherwise.</returns>
		bool InsertToMap(Key key, Value value);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="key"></param>
		/// <returns></returns>
		bool Find(const Key& key);

		/// <summary>
		/// Empties the map.
		/// </summary>
		void Clear();

		/// <summary>
		/// Gets the value of the inputted Key.
		/// </summary>
		/// <param name="key"></param>
		/// <returns>The key's value.</returns>
		Value& GetValue(Key key);
		const Value& GetValue(Key key) const;

	private:
		std::unordered_map<Key, Value> m_VariableMap;
	};

	template<typename Key, typename Value>
	inline bool VariableHolder<typename Key, typename Value>::InsertToMap(Key key, Value value)
	{
		return m_VariableMap.insert(std::make_pair(key, value));
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
