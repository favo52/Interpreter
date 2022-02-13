#include "pch.h"
#include "Interpreter.h"

#include "../Core/Application.h"

namespace Interpreter
{
	Interpreter::Interpreter() :
		m_LineNumber { 1 },
		m_IntValue{ 0 },
		m_RealValue{ 0.0 },
		m_IsExponential{ false },
		m_IsOperatorFound{ false }
	{
	}

	bool Interpreter::OpenFile(const std::string& filepath)
	{
		m_iFileStream.open(filepath);
		if (!m_iFileStream)
		{
			LOG_ERROR("Unable to open file: '{0}'", filepath);
			return false;
		}

		LOG_INFO("'{0}' opened successfully.", filepath);
		return true;
	}

	void Interpreter::ReadFile()
	{
		// Traverse the file line by line until the end
		while (std::getline(m_iFileStream, m_Line))
		{
			// TODO: Stop if end keyword is found
			InterpretLine(m_Line);
			m_LineNumber++;
		}
	}

	void Interpreter::InterpretLine(const std::string& line)
	{
		std::istringstream iss{ line };
		std::string word{};
		iss >> word;

		WordType wordType = GetWordType(word);
		switch (wordType)
		{
			case WordType::Keyword:
			{
				// Get keyword type
				KeywordType keywordType = Keyword::GetKeyword(word);

				// Grab everything after the keyword
				std::string expression = GetExpression(iss);
				ValidateKeyword(keywordType, expression);

				break;
			}

			case WordType::Variable:
			{
				VariableType varType = Variable::GetVariableType(word);

				// Save the variable name
				std::string variableStr{ word };

				// Make sure the assignment '=' is next in the line
				iss >> word;
				if (word != "=")
				{
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR("Did you forget the assignment '=' for the variable?");
				}

				// Grab the expression after the '='
				std::string expression = GetExpression(iss);

				// Make sure it's a valid assignment
				ValidateAssignment(varType, expression);

				// Store the variable and its value
				MakeAssignment(varType, variableStr, expression);
				break;
			}

			case WordType::Operator:
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("'" + word + "' An <operator> is not a valid instruction!");
				break;

			case WordType::Number:
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("'" + word + "' A <number> is not a valid instruction!");
				break;

			case WordType::String:
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("'" + word + "' A <string> is not a valid instruction!");
				break;

			case WordType::Invalid:
			default:
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("'" + word + "' is not supported!");
				break;
		}

		// GOAL:
		/*
		a = 2
		b = 2
		c = a .add. b
		*/

		// a = 2
		//Variable::m_IntHolder.InsertToMap(word, expression);

		// b = 2
		//m_IntHolder.InsertToMap("b", 2);

		// c = a .add. b
		//int A = m_IntHolder.GetValue("a");
		//int b = m_IntHolder.GetValue("b");
		//Variable::m_IntHolder.InsertToMap("c", a + b);

		// c = a .add. b .div. 5 .sub. 2
		//Variable::m_IntHolder.InsertToMap("c", a + b / 5 - 2);

		// a + b / 5 - 2
		//.add. +
		//std::string infixExpr = { a };
		//std::string postfixExptr = infixToPostfix(infixExpr);

		//evaluate_postfix(stack, postfixExptr);
	}

	void Interpreter::Reset()
	{
		m_iFileStream.clear();
		m_Line.clear();
		m_LineNumber = 1;

		m_IntValue = 0;
		m_RealValue = 0.0;
		m_IsExponential = false;

		ClearHolders();
		ClearStacks();
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
			{
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("String is missing ending quotation marks!");
			}

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
		for (const char& c : word)
		{
			if (c == '.')
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
			}
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

	void Interpreter::ValidateKeyword(KeywordType keywordType, std::string expression)
	{
		switch (keywordType)
		{
			// REM this is a comment
			case KeywordType::Comment:
				return;

			case KeywordType::If:
			{
				LOG_INFO("<if statement> '{0}'", expression);

				std::string tempStr{ expression };
				for (char& c : tempStr) c = std::tolower(c);
				size_t found = tempStr.find("then");
				if (found == std::string::npos)
				{
					ERROR("Did you forget the 'then' keyword?");
				}
				else
				{
					std::istringstream iss{ expression };
					std::string conditionalExpression;
					for (std::string tempWord; iss >> tempWord;)
					{
						if (Keyword::GetKeyword(tempWord) == KeywordType::Then)
							break;

						conditionalExpression += tempWord + ' ';
					}
					conditionalExpression.pop_back(); // Pop the last whitespace

					LOG_TRACE("Conditional expression: '{0}'", conditionalExpression);

					// Guard against double operators
					std::istringstream cond{ conditionalExpression };
					for (std::string s; cond >> s;)
					{
						if (Operator::IsOperator(s))
						{
							cond >> s;
							if (Operator::IsOperator(s))
								ERROR("Did you write 2 operators in a row?");
						}
					}

					std::string thenStatement{};
					for (std::string tempWord; iss >> tempWord;)
						thenStatement += tempWord + ' ';
					thenStatement.pop_back();

					ValidateKeyword(KeywordType::Then, thenStatement);
				}

				break;
			}

			case KeywordType::Then:
			{
				LOG_TRACE("then statement: '{0}'", expression);

				std::istringstream iss{ expression };
				std::string firstWord{};
				iss >> firstWord;

				if (Keyword::IsKeyword(firstWord))
				{
					KeywordType type = Keyword::GetKeyword(firstWord);
					if (type == KeywordType::Print)
					{
						std::string expr = GetExpression(iss);
						ValidateKeyword(type, expr);
						return;
					}
					else if (type == KeywordType::Read)
					{
						std::string expr = GetExpression(iss);
						ValidateKeyword(type, expr);
						return;
					}
				}
				else
				{
					std::string str{};
					iss >> str;
					if (str != "=") ERROR("Did you forget the assignment '=' for the variable?");

					// Grab the expression after the '='
					std::string expr = GetExpression(iss);

					ValidateAssignment(Variable::GetVariableType(firstWord), expr);
				}

				break;
			}

			case KeywordType::Read:
			{
				// Validate that expression is a variable

				// Make assignment to variable

				break;
			}

			case KeywordType::Print:
			{
				if (!expression.empty())
				{
					if (Variable::IsVariable(expression))
					{
						for (char c : expression)
						{
							if (std::isdigit((char)c))
							{
								LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
								ERROR("'" + expression + "' Variable names can not have any digits!");
								return;
							}
						}

						if (Keyword::IsKeyword(expression))
						{
							LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
							ERROR("Invalid print statement! Can't print a keyword!");
						}

						PrintVariable(expression);
						return;
					}

					if (IsNumber(expression))
					{
						std::cout << expression;
						return;
					}

					if (IsString(expression))
					{
						std::cout << expression;
						return;
					}
				}
				else
				{
					std::cout << '\n';
					return;
				}

				break;
			}

			case KeywordType::End:
			{
				/*
				if (!expression.empty())
				{
					LOG_TRACE("<end statement>");
					LOG_ERROR("'{0}' not executed.", expression);
					return;
				}
				*/

				//LOG_INFO("Program end");

				exit(EXIT_SUCCESS);
			}

			case KeywordType::Invalid: LOG_ERROR("Invalid Keyword Type"); break;
			default: LOG_ERROR("Unknown KeywordType!"); break;
		}
	}

	void Interpreter::ValidateAssignment(VariableType varType, std::string expression)
	{
		std::string word{};
		std::istringstream iss{ expression };
		iss >> word;

		// Verify that it is a valid assignment
		switch (varType)
		{
			case VariableType::Integer:
			{
				if (!IsInteger(word) || IsReal(word))
				{
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR("Invalid assignment to <integer> variable.");
				}
				break;
			}

			case VariableType::Real:
			{
				if (!IsReal(word) || IsInteger(word))
				{
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR("Invalid assignment to <real> variable.");
				}
				break;
			}

			case VariableType::String:
			{
				if (!IsString(expression))
				{
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR("Invalid assignment to <string> variable.");
				}
				break;
			}
		}

		// Verify arithmetic expressions

		// Grab words until iss is empty
		while (iss >> word)
		{
			if (Operator::IsOperator(word))
			{
				OperatorType op = Operator::GetOperator(word);
				if (Operator::IsLogical(op))
				{
					// Error Example: a = 5 .and. 3
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR(std::string("Used logical operator '" + word + "' in an assignment statement!").c_str());
				}
				if (Operator::IsRelational(op))
				{
					// Error Example: a = 5 .le. 3
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR(std::string("Used relational operator '" + word + "' in an assignment statement!").c_str());
				}

				// Grab next word
				iss >> word;
				if (Operator::IsOperator(word))
				{
					// Error Example: a = 5 .add. .div. 5
					LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
					ERROR(std::string("Was expecting a Number or Variable but found '" + word + "'.\nDid you write two operators in a row? ").c_str());
				}
			}
		}
	}

	void Interpreter::PrintVariable(const std::string& expression)
	{
		switch (Variable::GetVariableType(expression))
		{
			case VariableType::Integer: std::cout << m_IntHolder.GetValue(expression); break;
			case VariableType::Real: std::cout << m_RealHolder.GetValue(expression); break;
			case VariableType::String: std::cout << m_StringHolder.GetValue(expression); break;
		}
	}

	void Interpreter::MakeAssignment(VariableType varType, const std::string& variable, const std::string& expression)
	{
		switch (varType)
		{
			case VariableType::Integer:
			{
				m_IntHolder.InsertToMap(variable, m_IntValue);
				//LOG_INFO("m_IntValue {0}", m_IntValue);
				//LOG_INFO("m_IntHolder {0}", m_IntHolder.GetValue(variable));
			}
			break;

			case VariableType::Real:
			{
				m_RealHolder.InsertToMap(variable, m_RealValue);
			}
			break;

			case VariableType::String:
			{
				m_StringHolder.InsertToMap(variable, expression);
			}
			break;

			case VariableType::Invalid:
			default:
				LOG_ERROR("Line {0}: {1}", m_LineNumber, m_Line);
				ERROR("Cannot assign " + expression + " to " + variable + "!");
				break;
		}
	}

	void Interpreter::ClearHolders()
	{
		m_IntHolder.Clear();
		m_RealHolder.Clear();
		m_StringHolder.Clear();
	}

	void Interpreter::ClearStacks()
	{
		m_IntStack.Clear();
		m_RealStack.Clear();
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
		if (!expression.empty() && expression.front() == ' ')
			expression.erase(expression.begin()); // erase front whitespace

		return expression;
	}
}
