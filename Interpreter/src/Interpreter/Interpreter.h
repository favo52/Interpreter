#pragma once

#include "Core/VariableHolder.h"

namespace Interpreter
{
	class Interpreter
	{
	public:
		Interpreter();
		virtual ~Interpreter() = default;

		// Phase 1
		void ReadWord(std::string word);
		
		// Phase 2
		void ReadLine();

		// Phase 3
		void ReadFile();

		// File management
		bool LoadFile(const std::string& filepath);
		void CloseFile() { m_iFileStream.close(); }

		void Reset();

	private:
		enum class Keyword
		{
			Invalid = 0,
			Comment,
			If,
			Then,
			Read,
			Print,
			End
		};

		enum class Operator
		{
			Invalid = 0,
			Add, Sub, Mul, Div,		// Arithmetic
			Or, And, Not, 			// Logical
			Eq, Ne, Lt, Le, Gt, Ge	// Relational
		};

		enum class VariableType
		{
			Invalid = 0,
			Integer,
			Real,
			String
		};

	private:
		bool PrintKeyword(std::istringstream& iss, std::string& word);

		bool IsKeyword(std::string word);
		bool IsOperator(std::string word);
		bool IsVariable(const std::string& str);

		// Numbers
		bool IsIntegerNumber(const std::string& word);
		bool IsRealNumber(const std::string& word);
		bool IsSignedNumber(std::string word);
		bool IsDecimalPoint(const std::string& word);
		bool IsExponentNumber(const std::string& word);
		
		bool IsArithmetic(const Operator& op);
		bool IsRelational(const Operator& op);
		bool IsLogical(const Operator& op);

		bool IsVariableStored(const std::string& word);
		bool IsConditionalExprTrue(const std::string condExpr);

		bool ValidateIf();

		bool MakeAssignment(const std::string& variable, const std::string& expression);

		void PrintUserVariable(const std::string& variable);

		// Phase 1 function
		void PrintNumberRules(const std::string& word);
		//

		Keyword GetKeyword(std::string word);
		Operator GetOperator(std::string word);
		VariableType GetVariableType(const std::string& word);

	private:
		// File management
		std::ifstream m_iFileStream;
		std::string m_Line;
		uint32_t m_LineNumber;

		// User variables
		IntegerHolder m_IntegerHolder;
		RealHolder m_RealHolder;
		StringHolder m_StringHolder;

		// Hash Tables
		std::unordered_map<std::string, Keyword> m_KeywordMap;
		std::unordered_map<std::string, Operator> m_OperatorMap;
	};
}

// BROKEN:
// 5.5ee5
// 5.5e-4
// .5
