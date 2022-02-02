#include "pch.h"
#include "Operator.h"

#include "../Core/Application.h"

namespace Interpreter
{
	std::unordered_map<std::string, OperatorType> Operator::m_OperatorMap =
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

	bool Operator::IsOperator(std::string word)
	{
		if (word.front() == '.' && word.back() != '.')
			ERROR("Operator is missing ending point!");

		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search for the string in the hash map
		auto x = m_OperatorMap.find(word);
		if (x != m_OperatorMap.end())
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

	OperatorType Operator::GetOperator(std::string word)
	{
		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search hash map for the OperatorType
		auto x = m_OperatorMap.find(word);
		if (x != m_OperatorMap.end())
			return x->second;
		else
			return OperatorType::Invalid;
	}
}
