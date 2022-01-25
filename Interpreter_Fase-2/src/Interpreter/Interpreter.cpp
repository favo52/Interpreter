#include "pch.h"
#include "Interpreter.h"

#include "Core/Application.h"

namespace Interpreter
{
	void Interpreter::InterpretLine(const std::string& line)
	{
		m_Line = line;

		std::istringstream iss{ line };
		std::string word{};
		iss >> word; 

		if (Keyword::IsKeyword(word))
		{
			LOG_TRACE("Keyword");
			KeywordType keywordType = Keyword::GetKeyword(word);

			std::string expression = GetExpression(iss);
			Keyword::ValidateKeyword(keywordType, expression);

			//PrintLine();
		}
		else if (Variable::IsVariable(word) && !Keyword::IsKeyword(word))
		{
			VariableType varType = Variable::GetVariableType(word);

			iss >> word;
			if (word != "=") ERROR("Did you forget the assignment '=' for the variable?");

			std::cout << '\n';
			LOG_TRACE("<assignment statement>");

			// Grab the expression after the '='
			std::string expression = GetExpression(iss);
			ValidateAssignment(varType, expression);

			std::cout << '\n';
			std::istringstream stream{ expression };
			for (std::string w; stream >> w;)
				ReadWord(w);
		}
		else
		{
			if (IsNumber(word))
				ERROR("A Number is not a valid instruction!");

			if (IsString(word))
				ERROR("A String is not a valid instruction!");

			ReadWord(word);
		}
	}

	void Interpreter::ReadWord(std::string word)
	{
		WordType wordType = GetWordType(word);
		if (wordType != WordType::Invalid)
			LOG_INFO("'{0}' is a valid word!", word);

		switch (wordType)
		{
			case WordType::Keyword:
			{
				LOG_TRACE("It's a KEYWORD.");
				KeywordType keyword = Keyword::GetKeyword(word);
				PrintKeyword(keyword);
				break;
			}

			case WordType::Variable:
			{
				LOG_TRACE("It's a Variable.");
				VariableType varType = Variable::GetVariableType(word);
				PrintVariable(varType);
				break;
			}

			case WordType::Number:
				PrintNumber(word);
				break;

			case WordType::String:
				LOG_TRACE("It's a String.");
				PrintString(word);
				break;

			case WordType::Operator:
			{
				OperatorType op = Operator::GetOperator(word);
				if (Operator::IsArithmetic(op)) LOG_TRACE("<arithmetic operator>");
				if (Operator::IsRelational(op)) LOG_TRACE("<relational operator>");
				if (Operator::IsLogical(op)) LOG_TRACE("<logical operator>");
				PrintOperator(op);
				break;
			}

			case WordType::Invalid:
				LOG_ERROR("'{0}' is an invalid word!", word);
				break;

			default:
				LOG_ERROR("Unknown WordType!");
				break;
		}
	}

	void Interpreter::Reset()
	{
		m_Line.clear();

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

	void Interpreter::ValidateAssignment(VariableType varType, std::string expression)
	{
		std::string word{};
		std::istringstream iss{ expression };
		iss >> word;
		switch (varType)
		{
			case VariableType::Integer:
			{
				if (!IsInteger(word) || IsReal(word))
					ERROR("Invalid assignment to <integer> variable.");
				break;
			}

			case VariableType::Real:
			{
				if (!IsReal(word) || IsInteger(word))
					ERROR("Invalid assignment to <real> variable.");
				break;
			}

			case VariableType::String:
			{
				if (!IsString(word))
					ERROR("Invalid assignment to <string> variable.");
				break;
			}
		}
	}

	void Interpreter::PrintKeyword(KeywordType keyword)
	{
		switch (keyword)
		{
			case KeywordType::Comment: LOG_TRACE("<comment statement>"); break;
			case KeywordType::If: LOG_TRACE("<if statement>"); break;
			case KeywordType::Then: LOG_TRACE("<then statement>"); break;
			case KeywordType::Read: LOG_TRACE("<read statement>"); break;
			case KeywordType::Print: LOG_TRACE("<print statement>"); break;
			case KeywordType::End: LOG_TRACE("<end statement>"); break;
			case KeywordType::Invalid: LOG_ERROR("<invalid>"); break;

			default:
				LOG_ERROR("Unknown Keyword!"); break;
		}
	}

	void Interpreter::PrintVariable(VariableType varType)
	{
		switch (varType)
		{
			case VariableType::Integer: LOG_TRACE("<integer> variable."); break;
			case VariableType::Real: LOG_TRACE("<real> variable."); break;
			case VariableType::String: LOG_TRACE("<string> variable."); break;
			case VariableType::Invalid: LOG_ERROR("<invalid>"); break;

			default:
				LOG_ERROR("Unknown VariableType!") break;
		}
	}

	void Interpreter::PrintNumber(const std::string& word)
	{
		if (IsInteger(word))
		{
			LOG_TRACE("It's an <integer> number.");
			if (m_IntValue >= 0) { LOG_TRACE("<unsign integer>"); }
			else if (m_IntValue < 0) LOG_TRACE("<sign><unsign integer>");
		}
		else
		{
			LOG_TRACE("It's a <real> number.");
			if (m_RealValue >= 0.0f)
			{
				LOG_TRACE("<unsign integer><decimal part>");
			}
			else if (m_RealValue < 0.0f)
				LOG_TRACE("<sign><unsign integer><decimal part>");

			if (m_IsExponential)
				LOG_TRACE("<exponent part>");
		}
	}

	void Interpreter::PrintString(const std::string& word)
	{
		if (word.size() > 2)
		{
			LOG_TRACE("<any string><any sequence of character>");
		}
		else
		{
			LOG_TRACE("<string> \"\"");
		}
	}

	void Interpreter::PrintOperator(OperatorType op)
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

	WordType Interpreter::GetWordType(std::string word)
	{
		if (Keyword::IsKeyword(word)) return WordType::Keyword;
		if (Variable::IsVariable(word)) return WordType::Variable;
		if (IsNumber(word)) return WordType::Number;
		if (IsString(word)) return WordType::String;
		if (Operator::IsOperator(word)) return WordType::Operator;

		return WordType::Invalid;
	}

	std::string Interpreter::GetExpression(std::istringstream& iss)
	{
		std::string expression{};
		std::getline(iss, expression);
		if (expression.front() == ' ')
			expression.erase(expression.begin()); // erase front

		return expression;
	}
}
