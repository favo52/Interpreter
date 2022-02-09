#pragma once

#include <vector>

namespace Interpreter
{
	template<class ItemType>
	class Stack
	{
	public:
		Stack() = default;

		const ItemType& Peek() const;
		void Push(const ItemType& newEntry);
		void Pop();
		bool IsEmpty() const;
		void Clear();

	private:
		std::vector<ItemType> m_Items{};
	};

	template<class ItemType>
	inline const ItemType& Stack<ItemType>::Peek() const
	{
		assert(!IsEmpty());

		return m_Items.back();
	}

	template<class ItemType>
	inline void Stack<ItemType>::Push(const ItemType& newEntry)
	{
		m_Items.push_back(newEntry);
	}

	template<class ItemType>
	inline void Stack<ItemType>::Pop()
	{
		m_Items.pop_back();
	}

	template<class ItemType>
	inline bool Stack<ItemType>::IsEmpty() const
	{
		return m_Items.empty();
	}

	template<class ItemType>
	inline void Stack<ItemType>::Clear()
	{
		m_Items.clear();
	}

	typedef Stack<int> IntegerStack;
	typedef Stack<double> RealStack;
}
