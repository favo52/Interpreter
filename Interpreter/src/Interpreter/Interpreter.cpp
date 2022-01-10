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
			LOG_ERROR("Unable to open file \"{0}\"", filepath);
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
			//LOG_INFO("Line {0}: {1}", m_LineNumber, m_Line);
			std::istringstream iss{ m_Line };

			// Traverse the line
			for (std::string str; iss >> str;)
			{
				// Check for user defined variables
				if (IsVariable(str) && !IsKeyword(str))
				{
					// Check if it's an assignment
					std::string op; // operator
					iss >> op;
					if (op == "=")
					{
						std::string expression;
						iss >> expression;
						if (!MakeAssignment(str, expression)) return;
						break;
					}
					else
					{
						// error, needs = to be assignment
						LOG_ERROR("ERROR: Line {0}: {1}.", m_LineNumber, m_Line);
						return;
					}
				}
				else if (IsKeyword(str))
				{
					// deal with keywords
					Keyword keyword = GetKeyword(str);
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
							// Check if keyword is alone or has a following statement
							std::string statement;
							if (iss >> statement) // Has a statement
							{
								if (statement.front() == '\"')
								{
									statement.erase(statement.begin()); // delete first "
									if (statement.back() == '\"')
									{
										statement.pop_back(); // delete ending "
										LOG_INFO(statement);
									}
									else
									{
										LOG_ERROR("ERROR: Line {0}: {1}.", m_LineNumber, m_Line);
										LOG_ERROR("Missing ending '\"'");
									}
								}
								else if (IsVariableStored(statement)) // Print user variable
								{
									PrintUserVariable(statement);
								}
								else // Error
								{
									LOG_ERROR("ERROR: Line {0}: {1}.", m_LineNumber, m_Line);
									LOG_ERROR("Print error.");
								}
							}
							else // It's the keyword only
							{
								LOG_INFO("");
							}

							break;
						}
						case Keyword::End:
						{
							Application& app = Application::Get();
							LOG_INFO("end. reached");
							app.Quit();
							break;
						}

						default:
						{
							LOG_ERROR("ERROR: Line {0}: {1}.\n\t\t\"{2}\" Keyword is not implemented yet.", m_LineNumber, m_Line, str);
							return;
						}
					}					
				}
				else
				{
					// not a variable or keyword, error?
					LOG_ERROR("ERROR: Line {0}: {1}. Unknown.", m_LineNumber, m_Line);
					return;
				}
			}

			++m_LineNumber;
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
				// Validate integer expression
				std::size_t* position{ nullptr };
				LOG_WARN(expression);
				int value = std::stoi(expression);
				LOG_WARN(value);
				//if (position != nullptr)
				if (std::isdigit(value) != 0)
				{
					m_IntegerHolder.InsertToMap(variable, value);
					return true;
				}
				else // Invalid expression
				{
					LOG_ERROR("ERROR: Line {0}: {1}.", m_LineNumber, m_Line);
					LOG_ERROR("{0} is not an integer number!", expression);
					return false;
				}

				return false;
			}

			case VariableType::Real:
			{
				// Validate double expression
				std::size_t* position{ nullptr };
				double value = std::stod(expression, position);
				if (position != nullptr)
				{
					m_RealHolder.InsertToMap(variable, value);
					return true;
				}
				else // Invalid expression
				{
					LOG_ERROR("ERROR: Line {0}: {1} is not a real number!", m_LineNumber, expression);
					return false;
				}

				return false;
			}

			case VariableType::String:
			{
				// Validate string expression
				if (expression.front() == '"')
				{
					std::string str{ m_Line };
					int found = str.find_first_of('"');
					str.erase(0, found + 1);

					if (str.back() == '"')
					{
						str.pop_back();
						m_StringHolder.InsertToMap(variable, str);
						LOG_INFO("[stringholder] {0} = {1}", variable, m_StringHolder.GetValue(variable));
						return true;
					}
					else
					{
						LOG_ERROR("ERROR: Line {0}: {1}: String missing \" at the end.", m_LineNumber, m_Line);
						return false;
					}
					
					return false;
				}
				else
				{
					LOG_ERROR("ERROR: Line {0}: {1}: Missing \" in the font!", m_LineNumber, m_Line);
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
				LOG_INFO(m_IntegerHolder.GetValue(variable));
				break;

			case VariableType::Real:
				LOG_INFO(m_RealHolder.GetValue(variable));
				break;

			case VariableType::String:
				LOG_INFO(m_StringHolder.GetValue(variable));
				break;

			case VariableType::Invalid:
				LOG_ERROR("Variable Invalid");
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
			case 'K': case 'L': case 'M': case 'N': case 'Ñ':
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
