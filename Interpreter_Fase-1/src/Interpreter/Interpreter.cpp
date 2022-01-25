#include "pch.h"
#include "Interpreter.h"

#include "Core/Application.h"

namespace Interpreter
{
	Interpreter::Interpreter()
	{
		m_KeywordMap =
		{
			{"REM", Keyword::Comment},
			{"IF", Keyword::If},
			{"THEN", Keyword::Then},
			{"READ", Keyword::Read},
			{"PRINT", Keyword::Print},
			{"END.", Keyword::End}
		};

		m_OperatorMap =
		{
			{".add.", Operator::Add},
			{".sub.", Operator::Sub},
			{".mul.", Operator::Mul},
			{".div.", Operator::Div},
			{".or.", Operator::Or},
			{".and.", Operator::And},
			{".not.", Operator::Not},
			{".eq.", Operator::Eq},
			{".ne.", Operator::Ne},
			{".lt.", Operator::Lt},
			{".le.", Operator::Le},
			{".gt.", Operator::Gt},
			{".ge.", Operator::Ge}
		};
	}

	void Interpreter::ReadWord(std::string word)
	{
		WordType wordType = GetWordType(word);
		if (wordType != WordType::Invalid)
			LOG_INFO(word + " is a valid word!");

		switch (wordType)
		{
			case WordType::Keyword:
			{
				LOG_TRACE("It's a KEYWORD.")
				Keyword keyword = GetKeyword(word);
				PrintKeyword(keyword);
				break;
			}

			case WordType::Variable:
			{
				LOG_TRACE("It's a Variable.")
				VariableType varType = GetVariableType(word);
				PrintVariable(varType);
				break;
			}

			case WordType::Number:
				PrintNumber(word);
				break;

			case WordType::String:
				LOG_TRACE("It's a String.")
				PrintString(word);
				break;

			case WordType::Operator:
			{
				Operator op = GetOperator(word);
				if (IsArithmetic(op)) LOG_TRACE("<arithmetic operator>");
				if (IsRelational(op)) LOG_TRACE("<relational operator>");
				if (IsLogical(op)) LOG_TRACE("<logical operator>");				
				PrintOperator(op);
				break;
			}

			case WordType::Invalid:
				LOG_ERROR(word + " is an invalid word!");
				break;

			default:
				LOG_ERROR("Unknown WordType!");
				break;
		}
	}

	void Interpreter::Reset()
	{
		m_IntValue = 0;
		m_RealValue = 0.0f;
		m_IsExponential = false;
	}

	bool Interpreter::IsKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search hash map for the keyword
		auto x = m_KeywordMap.find(word);
		if (x != m_KeywordMap.end())
			return true;
		else
			return false;
	}

	bool Interpreter::IsVariable(const std::string& word)
	{
		for (const char& c : word)
			if (!isalpha(c)) // is not alphabet
				return false;

		return true;
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
				Error("ERROR: String is missing ending quotation marks!");

			return true;
		}

		return false;
	}

	bool Interpreter::IsOperator(std::string word)
	{
		if (word.front() == '.' && word.back() != '.')
			Error("ERROR: Operator is missing ending point!");

		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search hash map for the keyword
		auto x = m_OperatorMap.find(word);
		if (x != m_OperatorMap.end())
			return true;
		else
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
			m_RealValue = std::stof(word, &lastChar);
			LOG_INFO(std::to_string(m_RealValue));
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

	bool Interpreter::IsArithmetic(const Operator& op)
	{
		switch (op)
		{
			case Operator::Add:
			case Operator::Sub:
			case Operator::Mul:
			case Operator::Div:
				return true;

			default:
				return false;
		}
		return false;
	}

	bool Interpreter::IsRelational(const Operator& op)
	{
		switch (op)
		{
			case Operator::Gt:
			case Operator::Lt:
			case Operator::Eq:
			case Operator::Ge:
			case Operator::Le:
			case Operator::Ne:
				return true;

			default:
				return false;
		}
		return false;
	}

	bool Interpreter::IsLogical(const Operator& op)
	{
		switch (op)
		{
			case Operator::And:
			case Operator::Or:
			case Operator::Not:
				return true;

			default:
				return false;
		}
		return false;
	}

	void Interpreter::PrintKeyword(Keyword keyword)
	{
		switch (keyword)
		{
			case Keyword::Comment: LOG_TRACE("<comment statement>"); break;
			case Keyword::If: LOG_TRACE("<if statement>"); break;
			case Keyword::Then: LOG_TRACE("<then statement>"); break;
			case Keyword::Read: LOG_TRACE("<read statement>"); break;
			case Keyword::Print: LOG_TRACE("<print statement>"); break;
			case Keyword::End: LOG_TRACE("<end statement>"); break;
			case Keyword::Invalid: LOG_ERROR("<invalid>"); break;
			
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
			{ LOG_TRACE("<unsign integer><decimal part>"); }
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

	void Interpreter::PrintOperator(Operator op)
	{
		switch (op)
		{
			case Operator::Add: LOG_TRACE("It's a 'addition' (+) operation."); break;
			case Operator::Sub: LOG_TRACE("It's a 'subtraction' (-) operation."); break;
			case Operator::Mul: LOG_TRACE("It's a 'multiplication' (*) operation."); break;
			case Operator::Div: LOG_TRACE("It's a 'division' (/) operation."); break;
			case Operator::Or: LOG_TRACE("It's a 'or' (||) operation."); break;
			case Operator::And: LOG_TRACE("It's a 'and' (&&) operation."); break;
			case Operator::Not: LOG_TRACE("It's a 'not' (!) operation."); break;
			case Operator::Eq: LOG_TRACE("It's a 'equal' (==) operation."); break;
			case Operator::Ne: LOG_TRACE("It's a 'not equal' (!=) operation."); break;
			case Operator::Lt: LOG_TRACE("It's a 'less than' (<) operation."); break;
			case Operator::Le: LOG_TRACE("It's a 'less equal' (<=) operation."); break;
			case Operator::Gt: LOG_TRACE("It's a 'greater than' (>) operation."); break;
			case Operator::Ge: LOG_TRACE("It's a 'greater equal' (>=) operation."); break;
			case Operator::Invalid: LOG_ERROR("Invalid operation."); break;

		default:
			LOG_ERROR("Unknown Operator!"); break;
		}
	}

	WordType Interpreter::GetWordType(std::string word)
	{
		if (IsKeyword(word)) return WordType::Keyword;
		if (IsVariable(word)) return WordType::Variable;
		if (IsNumber(word)) return WordType::Number;
		if (IsString(word)) return WordType::String;
		if (IsOperator(word)) return WordType::Operator;

		return WordType::Invalid;
	}

	Keyword Interpreter::GetKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search hash map for the keyword
		auto x = m_KeywordMap.find(word);
		if (x != m_KeywordMap.end())
			return x->second;
		else
			return Keyword::Invalid;
	}

	Operator Interpreter::GetOperator(std::string word)
	{
		// Make all characters lowercase
		for (char& c : word)
			c = std::tolower(c);

		// Search hash map for the keyword
		auto x = m_OperatorMap.find(word);
		if (x != m_OperatorMap.end())
			return x->second;
		else
			return Operator::Invalid;
	}

	VariableType Interpreter::GetVariableType(const std::string& variable)
	{
		// Make the first letter uppercase
		char firstLetter{ variable.front() };
		firstLetter = std::toupper(firstLetter);

		switch (firstLetter)
		{
			case 'A': case 'B': case 'C':
			case 'D': case 'E':	case 'F':
				return VariableType::Integer;

			case 'G': case 'H': case 'I':
			case 'J': case 'K': case 'L':
			case 'M': case 'N': case 'Ñ':
				return VariableType::Real;

			case 'O': case 'P': case 'Q': case 'R':
			case 'S': case 'T': case 'U': case 'V':
			case 'W': case 'X': case 'Y': case 'Z':
				return VariableType::String;

			default:
				return VariableType::Invalid;
		}
	}
}
