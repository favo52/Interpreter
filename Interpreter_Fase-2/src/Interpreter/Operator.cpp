#include "pch.h"
#include "Operator.h"

#include "Core/Application.h"

namespace Interpreter
{
	std::unordered_map<std::string, OperatorType> Operator::m_OperatorMap{};

	Operator::Operator()
	{
		m_OperatorMap =
		{
			{".add.", OperatorType::Add},
			{".sub.", OperatorType::Sub},
			{".mul.", OperatorType::Mul},
			{".div.", OperatorType::Div},
			{".or.", OperatorType::Or},
			{".and.", OperatorType::And},
			{".not.", OperatorType::Not},
			{".eq.", OperatorType::Eq},
			{".ne.", OperatorType::Ne},
			{".lt.", OperatorType::Lt},
			{".le.", OperatorType::Le},
			{".gt.", OperatorType::Gt},
			{".ge.", OperatorType::Ge}
		};
	}

	bool Operator::IsOperator(std::string word)
	{
		if (word.front() == '.' && word.back() != '.')
			ERROR("Operator is missing ending point!");

		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search hash map for the Keywords
		auto x = m_OperatorMap.find(word);
		if (x != std::end(m_OperatorMap))
			return true;
		else
			return false;
	}

	bool Operator::IsArithmetic(const OperatorType& op)
	{
		switch (op)
		{
			case OperatorType::Add:
			case OperatorType::Sub:
			case OperatorType::Mul:
			case OperatorType::Div:
				return true;

			default:
				return false;
		}
		return false;
	}

	bool Operator::IsRelational(const OperatorType& op)
	{
		switch (op)
		{
			case OperatorType::Gt:
			case OperatorType::Lt:
			case OperatorType::Eq:
			case OperatorType::Ge:
			case OperatorType::Le:
			case OperatorType::Ne:
				return true;

			default:
				return false;
		}
		return false;
	}

	bool Operator::IsLogical(const OperatorType& op)
	{
		switch (op)
		{
			case OperatorType::And:
			case OperatorType::Or:
			case OperatorType::Not:
				return true;

			default:
				return false;
		}
		return false;
	}

	void Operator::InterpretOperator(OperatorType op)
	{
		switch (op)
		{
			case OperatorType::Add: LOG_TRACE("It's a 'addition' (+) operation."); break;
			case OperatorType::Sub: LOG_TRACE("It's a 'subtraction' (-) operation."); break;
			case OperatorType::Mul: LOG_TRACE("It's a 'multiplication' (*) operation."); break;
			case OperatorType::Div: LOG_TRACE("It's a 'division' (/) operation."); break;
			case OperatorType::Or: LOG_TRACE("It's a 'or' (||) operation."); break;
			case OperatorType::And: LOG_TRACE("It's a 'and' (&&) operation."); break;
			case OperatorType::Not: LOG_TRACE("It's a 'not' (!) operation."); break;
			case OperatorType::Eq: LOG_TRACE("It's a 'equal' (==) operation."); break;
			case OperatorType::Ne: LOG_TRACE("It's a 'not equal' (!=) operation."); break;
			case OperatorType::Lt: LOG_TRACE("It's a 'less than' (<) operation."); break;
			case OperatorType::Le: LOG_TRACE("It's a 'less equal' (<=) operation."); break;
			case OperatorType::Gt: LOG_TRACE("It's a 'greater than' (>) operation."); break;
			case OperatorType::Ge: LOG_TRACE("It's a 'greater equal' (>=) operation."); break;
			case OperatorType::Invalid: LOG_ERROR("Invalid operation."); break;

			default:
				LOG_ERROR("Unknown Operator!"); break;
		}
	}

	OperatorType Operator::GetOperator(std::string word)
	{
		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search hash map for the Keywords
		auto x = m_OperatorMap.find(word);
		if (x != std::end(m_OperatorMap))
			return x->second;
		else
			return OperatorType::Invalid;
	}
}
