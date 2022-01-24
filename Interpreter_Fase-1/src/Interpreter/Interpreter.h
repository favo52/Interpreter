#pragma once

namespace Interpreter
{
	enum class WordType
	{
		Invalid = 0,
		Keyword, Variable, Number, String, Operator
	};

	enum class Keyword
	{
		Invalid = 0,
		Comment, If, Then, Read, Print, End
	};

	enum class VariableType
	{
		Invalid = 0,
		Integer, Real, String
	};

	enum class Operator
	{
		Invalid = 0,
		Add, Sub, Mul, Div,		// Arithmetic
		And, Or, Not, 			// Logical
		Eq, Ne, Lt, Le, Gt, Ge	// Relational
	};

	class Interpreter
	{
	public:
		Interpreter();
		virtual ~Interpreter() = default;

		void ReadWord(std::string word);
		void Reset();

	private:
		// Word Types
		bool IsKeyword(std::string word);
		bool IsVariable(const std::string& word);
		bool IsNumber(const std::string& word);
		bool IsString(const std::string& word);
		bool IsOperator(std::string word);

		// Numbers
		bool IsInteger(std::string word);
		bool IsReal(std::string word);
		bool IsExponential(std::string word);

		// Operators
		bool IsArithmetic(const Operator& op);
		bool IsRelational(const Operator& op);
		bool IsLogical(const Operator& op);

		// Print messages
		void PrintKeyword(Keyword keyword);
		void PrintVariable(VariableType varType);
		void PrintNumber(const std::string& word);
		void PrintString(const std::string& word);
		void PrintOperator(Operator op);

		// Getters
		WordType GetWordType(std::string word);
		Keyword GetKeyword(std::string word);
		Operator GetOperator(std::string word);
		VariableType GetVariableType(const std::string& word);

	private:
		void Error(const std::string& s) { throw std::runtime_error(s); }

	private:
		int m_IntValue{ 0 };
		float m_RealValue{ 0.0f };
		bool m_IsExponential{ false };

		std::unordered_map<std::string, Keyword> m_KeywordMap;
		std::unordered_map<std::string, Operator> m_OperatorMap;
	};
}
