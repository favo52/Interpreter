#include "pch.h"
#include "Interpreter.h"

#include "Core/Application.h"

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

	bool Interpreter::LoadFile(const std::string& filepath)
	{
		m_iFileStream.open(filepath);
		if (!m_iFileStream)
		{
			LOG_ERROR("Unable to open file \"{0}\".", filepath);
			return false;
		}

		LOG_INFO("\"{0}\" opened successfully.", filepath);
		return true;
	}

	void Interpreter::ReadFile()
	{
		// Traverse the file line by line
		while (std::getline(m_iFileStream, m_Line))
		{
			//LOG_WARN("Line {0}: {1}", m_LineNumber, m_Line);
			std::istringstream iss{ m_Line };

			// Traverse the line word by word (whitespace-separated strings/chars)
			for (std::string word; iss >> word;)
			{
				// Check for user defined variables
				if (IsVariable(word) && !IsKeyword(word))
				{
					// Check if it's an assignment
					std::string op; // operator
					iss >> op;
					if (op == "=")
					{
						std::string expression;
						iss >> expression;
						if (!MakeAssignment(word, expression)) return;
						break;
					}
					else
					{
						// error, needs = to be assignment
						std::cout << '\n';
						LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
						LOG_ERROR("Assignment operator ('=') not found.")
						return;
					}
				}
				else if (IsKeyword(word))
				{
					// deal with keywords
					Keyword keyword = GetKeyword(word);
					switch (keyword)
					{
						case Keyword::Comment:
						{
							break;
						}
						case Keyword::If:
						{
							break;
						}
						case Keyword::Then:
						{
							break;
						}
						case Keyword::Read:
						{
							break;
						}
						case Keyword::Print:
						{
							if (!PrintKeyword(iss, word))
								return;

							break;
						}
						case Keyword::End:
						{
							Application& app = Application::Get();
							std::cout << '\n';
							LOG_INFO("Program End");
							//app.Quit();
							break;
						}

						default:
						{
							std::cout << '\n';
							LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
							LOG_ERROR("\t\t\"{0}\" Keyword is not implemented yet.", word);
							return;
						}
					}
					break;
				}
				else
				{
					// Not a variable or keyword, error
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					LOG_ERROR("Unrecognized statement. Not a user variable or keyword!");
					return;
				}
			}

			++m_LineNumber;
		}
	}

	void Interpreter::Reset()
	{
		m_LineNumber = 1;
		m_iFileStream.clear();
		m_Line.clear();
		m_IntegerHolder.Clear();
		m_RealHolder.Clear();
		m_StringHolder.Clear();
	}

	bool Interpreter::PrintKeyword(std::istringstream& iss, std::string& word)
	{
		// Check if keyword is alone or has a following statement
		std::string statement;
		if (iss >> statement) // Has a statement
		{
			// Deal with strings
			if (statement.front() == '\"')
			{
				word = m_Line;
				int found = word.find_first_of('"');
				word.erase(0, found + 1);

				if (word.back() == '\"')
				{
					word.pop_back(); // delete ending "
					std::cout << word;
				}
				else
				{
					std::cout << '\n';
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					LOG_ERROR("String is missing ending quotation mark ('\"').");
					return false;
				}
			}
			else if (IsNumber(statement))
			{	// Print numbers
				std::cout << statement;
			}
			else // Deal with variables
			{
				if (IsVariableStored(statement)) // Print user variable
				{
					PrintUserVariable(statement);
					return true;
				}
				else
				{
					std::cout << '\n';
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					if (statement.back() == '\"') {
						LOG_ERROR("'{0}' is missing the opening quotation mark ('\"')!", statement);
					}
					else
					{
						if (!IsVariable(statement))
						{
							LOG_ERROR("'{0}' is an invalid variable name!", statement);
							return false;
						}
						LOG_ERROR("'{0}' is not an initialized variable!", statement);
					}
					return false;
				}

				// All other errors
				std::cout << '\n';
				LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
				LOG_ERROR("Unknown Print error.");
				return false;
			}
		}
		else // Print a new line if it's the keyword only
		{
			std::cout << '\n';
		}
	}

	bool Interpreter::IsVariable(const std::string& str)
	{
		for (const char& c : str)
			if (!isalpha(c)) // is not alphabet
				return false;

		return true;
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

	bool Interpreter::IsNumber(const std::string& statement)
	{
		for (const char& c : statement)
			if (!isdigit(c))
				return false;

		return true;
	}
	
	bool Interpreter::IsVariableStored(const std::string& statement)
	{
		switch (GetVariableType(statement))
		{
			case VariableType::Integer:
				if (m_IntegerHolder.Find(statement))
					return true;
				break;

			case VariableType::Real:
				if (m_RealHolder.Find(statement))
					return true;
				break;

			case VariableType::String:
				if (m_StringHolder.Find(statement))
					return true;
				break;
		}

		return false;
	}

	bool Interpreter::MakeAssignment(const std::string& variable, const std::string& expression)
	{
		switch (GetVariableType(variable))
		{
			case VariableType::Integer:
			{
				for (const char& c : expression)
					if (!std::isdigit(c))
					{
						std::cout << '\n';
						LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
						if (c == '.') {
							LOG_ERROR("Integers cannot have a decimal point!");
						}
						else {
							LOG_ERROR("'{0}' is not an integer number!", expression);
						}
						return false;
					}

				// Validate integer expression
				int value{ 0 };
				try {
					value = std::stoi(expression);
				}
				catch (...) { // Invalid expression
					std::cout << '\n';
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					LOG_ERROR("'{0}' is not an integer number!", expression);
					return false;
				}

				m_IntegerHolder.InsertToMap(variable, value);
				return true;
			}

			case VariableType::Real:
			{
				bool decimalFound{ false };
				for (const char& c : expression)
					if (!std::isdigit(c))
					{
						if (c == '.' && !decimalFound)
						{
							decimalFound = true;
							continue;
						}
						std::cout << '\n';
						LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
						if (decimalFound) {
							LOG_ERROR("'{0}' has more than one decimal point!", expression);
						}
						else {
							LOG_ERROR("'{0}' is not a real number!", expression);
						}
						return false;
					}

				if (decimalFound)
				{
					// Validate double expression
					double value{ 0.0 };
					try {
						value = std::stod(expression);
					}
					catch (...) { // Invalid expression
						std::cout << '\n';
						LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
						LOG_ERROR("'{0}' is not a real number!", expression);
						return false;
					}

					m_RealHolder.InsertToMap(variable, value);
					return true;
				}
				else
				{
					std::cout << '\n';
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					LOG_ERROR("'{0}' is missing a decimal point!", expression);
					return false;
				}
				return false;
			}

			case VariableType::String:
			{
				// Validate string expression
				if (expression.front() == '"')
				{
					// Grab the user's string with quotation marks
					std::string str{ m_Line };
					int found = str.find_first_of('"');
					str.erase(0, found + 1);

					if (str.back() == '"')
					{
						str.pop_back();
						m_StringHolder.InsertToMap(variable, str);
						return true;
					}
					else
					{
						std::cout << '\n';
						LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
						LOG_ERROR("String is missing quotation mark ('\"') at the end!");
						return false;
					}
					
					return false;
				}
				else
				{
					std::cout << '\n';
					LOG_ERROR("ERROR: Line {0}: {1}", m_LineNumber, m_Line);
					LOG_ERROR("String is missing quotation mark ('\"') in the front!");
					return false;
				}
				return false;
			}

			case VariableType::Invalid:
			default:
				return false;
		}
	}

	void Interpreter::PrintUserVariable(const std::string& variable)
	{
		switch (GetVariableType(variable))
		{
			case VariableType::Integer:
				std::cout << m_IntegerHolder.GetValue(variable);
				break;

			case VariableType::Real:
				std::cout << m_RealHolder.GetValue(variable);
				break;

			case VariableType::String:
				std::cout << m_StringHolder.GetValue(variable);
				break;

			case VariableType::Invalid:
				std::cout << '\n';
				LOG_ERROR("Invalid Variable!");
				break;
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
			case 'K': case 'L': case 'M': case 'N': case '�':
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
