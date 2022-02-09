#pragma once

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

	private:
		friend class Interpreter;
	};
}
