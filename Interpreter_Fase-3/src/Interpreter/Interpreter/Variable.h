#pragma once

#include "../Core/VariableHolder.h"

namespace Interpreter
{
	enum class VariableType
	{
		Invalid = 0,
		Integer, Real, String
	};

	class Variable
	{
	public:
		static bool IsVariable(const std::string& word);
		static VariableType GetVariableType(const std::string& word);

		static void Clear();

	private:
		static IntegerHolder m_IntHolder;
		static RealHolder m_RealHolder;
		static StringHolder m_StringHolder;

		friend class Interpreter;
	};
}
