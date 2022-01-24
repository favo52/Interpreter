#include "pch.h"
#include "Interpreter.h"

#include "Core/Application.h"

#include "Keyword.h"
#include "Operator.h"
#include "Variable.h"

namespace Interpreter
{
	void Interpreter::InterpretLine(const std::string& line)
	{
		m_Line = line;
		m_ISS.str(line);

		for (std::string word; m_ISS >> word;)
		{
			WordType wordType = GetWordType(word);
			if (wordType != WordType::Invalid)
				LOG_INFO("'{0}' is a valid word!", word);

			switch (wordType)
			{
				case WordType::Keyword:
				{
					LOG_TRACE("It's a Keywords.");
					KeywordType keywordType = Keyword::GetKeyword(word);
					Keyword::InterpretKeyword(keywordType);
					break;
				}

				case WordType::Variable:
				{
					LOG_TRACE("It's a Variable.");
					VariableType varType = Variable::GetVariableType(word);

					m_ISS >> word;
					if (word != "=") ERROR("Did you forget the assignment '=' for the variable?");

					// Grab the expression
					std::string expression{};
					for (std::string str; m_ISS >> str;)
					{
						expression += str + ' ';
						if (m_ISS.eof()) expression.pop_back();
					}
					
					Variable::InterpretVariable(varType, expression);
					break;
				}

				case WordType::Number:
				{
					LOG_TRACE("It's a Number.");
					break;
				}

				case WordType::String:
				{
					LOG_TRACE("It's a String.");
					break;
				}

				case WordType::Operator:
				{
					LOG_TRACE("It's an Operator.");

					OperatorType op = Operator::GetOperator(word);
					if (Operator::IsArithmetic(op))
					{

					}

					if (Operator::IsRelational(op))
					{

					}

					if (Operator::IsLogical(op))
					{

					}
					
					break;
				}

				case WordType::Invalid:
					LOG_ERROR("'{0}' is an invalid word!", word);
					break;

				default:
					LOG_ERROR("Unknown WordType!");
					break;
			}

			//std::cout << std::endl;
		}
	}

	void Interpreter::Reset()
	{
		m_Line.clear();
		m_ISS.clear();

		m_IntValue = 0;
		m_RealValue = 0.0f;
		m_IsExponential = false;
	}

	bool Interpreter::IsNumber(const std::string& word)
	{
		if (IsInteger(word))
			return true;

		if (IsReal(word))
		{
			if (IsExponential(word))
				m_IsExponential = true;

			return true;
		}

		return false;
	}

	bool Interpreter::IsString(const std::string& word)
	{
		if (word.front() == '\"')
		{
			if (word.back() != '\"' || word.size() < 2)
				ERROR("String is missing ending quotation marks!");

			return true;
		}

		return false;
	}

	bool Interpreter::IsInteger(std::string word)
	{
		try
		{
			size_t lastChar{ 0 };
			m_IntValue = std::stoi(word, &lastChar);
			return lastChar == word.size();
		}
		catch (...)
		{
			return false;
		}

		return false;
	}

	bool Interpreter::IsReal(std::string word)
	{
		try
		{
			size_t lastChar{ 0 };
			m_RealValue = std::stod(word, &lastChar);

			return lastChar == word.size();
		}
		catch (...)
		{
			return false;
		}

		return false;
	}

	bool Interpreter::IsExponential(std::string word)
	{
		for (const char& ch : word)
			if (ch == 'e' || ch == 'E')
				return true;

		return false;
	}

	void Interpreter::InterpretNumber(const std::string& word)
	{
	}

	void Interpreter::InterpretString(const std::string& word)
	{

	}

	WordType Interpreter::GetWordType(std::string word)
	{
		if (Keyword::IsKeyword(word)) return WordType::Keyword;
		if (Variable::IsVariable(word)) return WordType::Variable;
		if (IsNumber(word)) return WordType::Number;
		if (IsString(word)) return WordType::String;
		if (Operator::IsOperator(word)) return WordType::Operator;

		return WordType::Invalid;
	}
}
