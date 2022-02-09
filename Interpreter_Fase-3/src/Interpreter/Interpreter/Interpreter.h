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
		Interpreter();
		virtual ~Interpreter() = default;

		bool OpenFile(const std::string& filepath);
		void CloseFile() { m_iFileStream.close(); }

		void ReadFile();

		void Reset();

	private:
		void InterpretLine(const std::string& line);

		// Word Types
		bool IsNumber(const std::string& word);
		bool IsString(const std::string& word);

		// Numbers
		bool IsInteger(std::string word);
		bool IsReal(std::string word);
		bool IsExponential(std::string word);

		// Validation
		void ValidateKeyword(KeywordType keywordType, std::string expression);
		void ValidateAssignment(VariableType varType, std::string expression);

		// Getters
		WordType GetWordType(std::string word);
		std::string GetExpression(std::istringstream& iss);

	private:
		std::ifstream m_iFileStream;
		std::string m_Line;
		int m_LineNumber;

		int m_IntValue;
		double m_RealValue;
		bool m_IsExponential;

		bool m_IsOperatorFound;

		friend class Keyword;
	};
}
