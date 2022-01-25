#pragma once

#include "Keyword.h"
#include "Operator.h"
#include "Variable.h"

namespace Interpreter
{
	enum class WordType
	{
		Invalid = 0,
		Keyword, Variable, Number, String, Operator
	};

	class Interpreter
	{
	public:
		Interpreter() = default;
		virtual ~Interpreter() = default;

		void InterpretLine(const std::string& line);
		void ReadWord(std::string word);
		void Reset();

	private:
		// Word Types
		bool IsNumber(const std::string& word);
		bool IsString(const std::string& word);

		// Numbers
		bool IsInteger(std::string word);
		bool IsReal(std::string word);
		bool IsExponential(std::string word);

		// Validation
		void ValidateAssignment(VariableType varType, std::string expression);

		// Print messages
		void PrintKeyword(KeywordType keyword);
		void PrintVariable(VariableType varType);
		void PrintNumber(const std::string& word);
		void PrintString(const std::string& word);
		void PrintOperator(OperatorType op);

		// Getters
		WordType GetWordType(std::string word);
		std::string GetExpression(std::istringstream& iss);

	private:
		std::string m_Line;

		int m_IntValue{ 0 };
		double m_RealValue{ 0.0 };
		bool m_IsExponential{ false };
	};
}
