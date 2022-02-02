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

		// File management
		void ReadFile();

		bool LoadFile(const std::string& filepath);
		void CloseFile() { m_iFileStream.close(); }

		//void ReadWord(std::string word);
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

		// Validation
		void ValidateKeyword(KeywordType keywordType, std::string expression);
		void ValidateAssignment(VariableType varType, std::string expression);

		// Getters
		WordType GetWordType(std::string word);
		std::string GetExpression(std::istringstream& iss);

	private:
		std::ifstream m_iFileStream;
		std::string m_Line;
		int m_LineNumber{ 1 };

		int m_IntValue{ 0 };
		double m_RealValue{ 0.0 };
		bool m_IsExponential{ false };

		bool m_IsOperatorFound{ false };

		friend class Keyword;
	};
}
