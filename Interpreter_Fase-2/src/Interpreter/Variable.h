#pragma once

#include "Core/VariableHolder.h"

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
		Variable() = default;
		virtual ~Variable() = default;

		static bool IsVariable(const std::string& word);

		static void InterpretVariable(VariableType varType, std::string expression);

		static VariableType GetVariableType(const std::string& word);

	private:
		static IntegerHolder m_IntHolder;
		static RealHolder m_RealHolder;
		static StringHolder m_StringHolder;

		friend class Interpreter;
	};
}
