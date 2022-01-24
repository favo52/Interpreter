#include "pch.h"
#include "Variable.h"

namespace Interpreter
{
    bool Variable::IsVariable(const std::string& word)
    {
        for (const char& c : word)
            if (!isalpha(c)) // is not alphabet
                return false;

        return true;
    }

    void Variable::InterpretVariable(VariableType varType, std::string expression)
	{
		switch (varType)
		{
			case VariableType::Integer:
			{

				break;
			}
			case VariableType::Real:
			{

				break;
			}
			case VariableType::String:
			{
				//if ()
				break;
			}
			case VariableType::Invalid:
				break;

			default:
				LOG_ERROR("Unknown VariableType!");
				break;
		}
	}

    VariableType Variable::GetVariableType(const std::string& word)
	{
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
}
