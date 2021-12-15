#include "pch.h"
#include "Interpreter.h"

namespace Interpreter
{
	Interpreter::Interpreter() :
		m_LineNumber{ 1 }
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
	}

	void Interpreter::LoadFile(const std::string& filepath)
	{
		m_iFileStream.open(filepath);
		if (!m_iFileStream) throw std::runtime_error("Unable to open file " + filepath);
	}

	void Interpreter::ReadFile()
	{
		// Traverse the file line by line
		while (std::getline(m_iFileStream, m_Line))
		{
			std::istringstream iss{ m_Line };

			// Traverse the line
			for (std::string str; iss >> str;)
			{
				// Check for user defined variables
				if (IsVariable(str) && !IsKeyword(str))
				{
					// Check if assignment
					std::string op;
					iss >> op;
					if (op == "=")
					{
						std::string expression;
						iss >> expression;
						if (!MakeAssignment(str, expression))
						{
							LOG_ERROR("ERROR: Line {0}", m_LineNumber);
							return;
						}
					}
					else
					{
						// error, needs = to be assignment
					}
				}
				else if (IsKeyword(str))
				{
					// deal with keywords
				}
				else
				{
					// not a variable or keyword, error?
				}
			}

			++m_LineNumber;
		}
	}

	bool Interpreter::IsVariable(const std::string& str)
	{
		for (const char& c : str)
			if (isalpha(c))
				return true;

		return false;
	}

	bool Interpreter::IsKeyword(std::string str)
	{
		// Make all characters uppercase
		for (char& c : str)
			c = std::toupper(c);

		// Search hash map for the keyword
		auto x = m_KeywordMap.find(str);
		if (x != std::end(m_KeywordMap))
			return true;
		else
			return false;
	}

	bool Interpreter::MakeAssignment(const std::string& variable, const std::string& expression)
	{
		switch (GetVariableType(variable))
		{
			case VariableType::Integer:
			{
				if (isdigit(std::stoi(expression)))
				{

				}
				break;
			}

			case VariableType::Real:
			{

			}

			case VariableType::String:
			{

			}

			case VariableType::Invalid:
			default:
				return false;
		}
	}

	Interpreter::Keyword Interpreter::GetKeyword(std::string str)
	{
		// Make all characters uppercase
		for (char& c : str)
			c = std::toupper(c);

		// Search hash map for the keyword
		auto x = m_KeywordMap.find(str);
		if (x != std::end(m_KeywordMap))
			return x->second;
		else
			return Keyword::Invalid;
	}

	Interpreter::VariableType Interpreter::GetVariableType(const std::string& variable)
	{
		// Make the first letter uppercase
		char firstLetter{ variable.front() };
		firstLetter = std::toupper(firstLetter);

		switch (firstLetter)
		{
			case 'A': case 'B': case 'C':
			case 'D': case 'E':	case 'F':
				return VariableType::Integer;

			case 'G': case 'H': case 'I': case 'J':
			case 'K': case 'L': case 'M': case 'N':
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
// resources/sampleLAO.txt