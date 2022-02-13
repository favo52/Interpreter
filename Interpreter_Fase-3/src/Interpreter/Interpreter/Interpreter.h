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

		void PrintVariable(const std::string& expression);
		void MakeAssignment(VariableType varType, const std::string& variable, const std::string& expression);

		int Precedence(std::string word);
		std::string InfixToPostfix(std::string infix);

		template<typename T>
		T Operate(OperatorType op, const T& operand1, const T& operand2);

		template<typename T>
		T EvaluatePostfix(std::string postfix);

		void ClearHolders();
		void ClearStacks();

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
	inline T Interpreter::Operate(OperatorType op, const T& operand1, const T& operand2)
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
	inline T Interpreter::EvaluatePostfix(std::string postfix)
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
								stack.Push(m_IntHolder.GetValue(word));
								break;
							case VariableType::Real:
								stack.Push(m_RealHolder.GetValue(word));
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

				result = Operate(op, operand1, operand2);
				stack.Push(result);
			}
		}

		return result;
	}
}
