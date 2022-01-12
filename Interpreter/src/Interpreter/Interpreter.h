#pragma once

#include "Core/VariableHolder.h"

namespace Interpreter
{
	class Interpreter
	{
	public:
		Interpreter();
		virtual ~Interpreter() = default;

		// File management
		bool LoadFile(const std::string& filepath);
		void CloseFile() { m_iFileStream.close(); }

		void ReadFile();

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

		bool IsKeyword(std::string statement);
		bool IsOperator(std::string statement);
		bool IsVariable(const std::string& str);
		bool IsNumber(const std::string& statement);

		bool IsArithmetic(const Operator& op);
		bool IsRelational(const Operator& op);
		bool IsLogical(const Operator& op);

		bool IsVariableStored(const std::string& statement);
		bool IsConditionalExprTrue(const std::string condExpr);

		bool ValidateIf();

		bool MakeAssignment(const std::string& variable, const std::string& expression);

		void PrintUserVariable(const std::string& variable);

		Keyword GetKeyword(std::string statement);
		Operator GetOperator(std::string statement);
		VariableType GetVariableType(const std::string& statement);

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
