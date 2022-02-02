#include "pch.h"
#include "Variable.h"

namespace Interpreter
{
	IntegerHolder Variable::m_IntHolder{};
	RealHolder Variable::m_RealHolder{};
	StringHolder Variable::m_StringHolder{};

	bool Variable::IsVariable(const std::string& word)
	{
		for (const char& c : word)
			if (!isalpha(c)) // is not alphabetic
				return false;

		return true;
	}

	VariableType Variable::GetVariableType(const std::string& word)
	{
		if (word.empty())
			return VariableType::Invalid;

		// Make the first letter uppercase
		char firstLetter{ word.front() };
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

	void Variable::Clear()
	{
		m_IntHolder.Clear();
		m_RealHolder.Clear();
		m_StringHolder.Clear();
	}
}
