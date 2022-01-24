#pragma once

namespace Interpreter
{
	enum class OperatorType
	{
		Invalid = 0,
		Add, Sub, Mul, Div,		// Arithmetic
		And, Or, Not, 			// Logical
		Eq, Ne, Lt, Le, Gt, Ge	// Relational
	};

	class Operator
	{
	public:
		Operator();
		virtual ~Operator() = default;

		static bool IsOperator(std::string word);
		static bool IsArithmetic(const OperatorType& op);
		static bool IsRelational(const OperatorType& op);
		static bool IsLogical(const OperatorType& op);

		// Or maybe interpret conditional statement?
		static void InterpretOperator(OperatorType op);

		static OperatorType GetOperator(std::string word);

	private:
		static std::unordered_map<std::string, OperatorType> m_OperatorMap;
	};
}
