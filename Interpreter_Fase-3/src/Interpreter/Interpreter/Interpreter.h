#pragma once

#include "Keyword.h"
#include "Operator.h"
#include "Variable.h"

#include "Interpreter/Core/VariableHolder.h"
#include "Interpreter/Core/Stack.h"

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

		const std::string& GetLine() const { return m_Line; }
		int GetLineNumber() const { return m_LineNumber; }

	private:
		bool IsSyntaxValid(const std::string& line);
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

		void PrintVariable(const std::string& expression);
		void MakeAssignment(VariableType varType, const std::string& variable, const std::string& expression);

		int Precedence(std::string word);
		std::string InfixToPostfix(std::string infix);

		template<typename T>
		T OperateArithmetic(OperatorType op, const T& operand1, const T& operand2);

		template<typename T>
		bool OperateLogicalAndRelational(OperatorType op, const T& operand1, const T& operand2);

		template<typename T>
		T EvaluateArithmeticPostfix(std::string postfix);

		template<typename T>
		bool EvaluateOperatorPostfix(std::string postfix);

		bool OperateRelational(OperatorType op, const std::string& operand1, const std::string& operand2);
		std::string EvaluateStringPostfix(std::string postfix);
		bool EvaluateStringOperatorPostfix(std::string postfix);
		std::string Concatenate(std::string expression);

		void ClearHolders();
		void ClearStacks();

		// Getters
		WordType GetWordType(std::string word);
		std::string GetExpression(std::istringstream& iss);
		std::string GetLastError();

	private:
		std::ifstream m_iFileStream;
		std::string m_Line;
		int m_LineNumber;
		std::string m_ErrorWord;

		int m_IntValue;
		double m_RealValue;
		std::string m_StringValue;
		bool m_IsExponential;

		bool m_IsOperatorFound;

		// Variable holders
		IntegerHolder m_IntHolder;
		RealHolder m_RealHolder;
		StringHolder m_StringHolder;

		// Stacks for arithmetic operations
		IntegerStack m_IntStack;
		RealStack m_RealStack;

		bool m_IsEnd;

		friend class Keyword;
	};

	template<typename T>
	inline T Interpreter::OperateArithmetic(OperatorType op, const T& operand1, const T& operand2)
	{
		switch (op)
		{
			case OperatorType::Add:
				return operand1 + operand2;
			case OperatorType::Sub:
				return operand1 - operand2;
			case OperatorType::Mul:
				return operand1 * operand2;
			case OperatorType::Div:
				return operand1 / operand2;
			default:
				break;
		}
	}

	template<typename T>
	inline bool Interpreter::OperateLogicalAndRelational(OperatorType op, const T& operand1, const T& operand2)
	{
		switch (op)
		{
			case OperatorType::And:
				return operand1 && operand2;
			case OperatorType::Or:
				return operand1 || operand2;
			case OperatorType::Not:
				return !operand1;

			case OperatorType::Eq:
				return operand1 == operand2;
			case OperatorType::Ne:
				return operand1 != operand2;
			case OperatorType::Lt:
				return operand1 < operand2;
			case OperatorType::Le:
				return operand1 <= operand2;
			case OperatorType::Gt:
				return operand1 > operand2;
			case OperatorType::Ge:
				return operand1 >= operand2;
		}
	}

	template<typename T>
	inline T Interpreter::EvaluateArithmeticPostfix(std::string postfix)
	{
		Stack<T> stack;
		T result{};
		std::string word{};
		std::istringstream iss{ postfix };

		while (iss >> word)
		{
			if (!Operator::IsOperator(word))
			{
				WordType wordType = GetWordType(word);
				switch (wordType)
				{
					case WordType::Number:
					{
						if (IsInteger(word))
							stack.Push(stoi(word));
						else if (IsReal(word))
							stack.Push(stod(word));
					}
					break;

					case WordType::Variable:
					{
						VariableType varType = Variable::GetVariableType(word);
						switch (varType)
						{
							case VariableType::Integer:
								if (m_IntHolder.Find(word))
									stack.Push(m_IntHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::Real:
								if (m_RealHolder.Find(word))
									stack.Push(m_RealHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;
						}
					}
					break;
				}
			}
			else
			{
				OperatorType op = Operator::GetOperator(word);

				T operand2 = stack.Peek();
				stack.Pop();

				T operand1 = stack.Peek();
				stack.Pop();

				result = OperateArithmetic(op, operand1, operand2);
				stack.Push(result);
			}
		}

		return result;
	}

	template<typename T>
	inline bool Interpreter::Interpreter::EvaluateOperatorPostfix(std::string postfix)
	{
		Stack<T> stack;
		bool result{ false };
		std::string word{};
		std::istringstream iss{ postfix };

		while (iss >> word)
		{
			if (!Operator::IsOperator(word))
			{
				WordType wordType = GetWordType(word);
				switch (wordType)
				{
					case WordType::Number:
					{
						if (IsInteger(word))
							stack.Push(stoi(word));
						else if (IsReal(word))
							stack.Push(stod(word));
					}
					break;

					case WordType::Variable:
					{
						VariableType varType = Variable::GetVariableType(word);
						switch (varType)
						{
							case VariableType::Integer:
								if (m_IntHolder.Find(word))
									stack.Push(m_IntHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::Real:
								if (m_RealHolder.Find(word))
									stack.Push(m_RealHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;
						}
					}
					break;
				}
			}
			else
			{
				OperatorType op = Operator::GetOperator(word);

				T operand2 = stack.Peek();
				stack.Pop();

				T operand1 = stack.Peek();
				stack.Pop();

				result = OperateLogicalAndRelational(op, operand1, operand2);
				stack.Push(result);
			}
		}

		return result;
	}
}
