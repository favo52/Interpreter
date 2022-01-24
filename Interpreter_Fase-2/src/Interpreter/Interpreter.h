#pragma once

#include "Calculator/Calculator.h"

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
		void Reset();

	private:
		// Word Types
		bool IsNumber(const std::string& word);
		bool IsString(const std::string& word);

		// Numbers
		bool IsInteger(std::string word);
		bool IsReal(std::string word);
		bool IsExponential(std::string word);

		// Interpret
		void InterpretNumber(const std::string& word);
		void InterpretString(const std::string& word);

		// Getters
		WordType GetWordType(std::string word);

	private:
		std::string m_Line;
		std::istringstream m_ISS;

		int m_IntValue{ 0 };
		double m_RealValue{ 0.0 };
		bool m_IsExponential{ false };

		Calculator m_Calculator;
	};
}
