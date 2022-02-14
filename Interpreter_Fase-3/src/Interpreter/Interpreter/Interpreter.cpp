#include "pch.h"
#include "Interpreter.h"

#include "../Core/Application.h"

namespace Interpreter
{
	Interpreter::Interpreter() :
		m_LineNumber{ 1 },
		m_IntValue{ 0 },
		m_RealValue{ 0.0 },
		m_IsExponential{ false },
		m_IsOperatorFound{ false },
		m_IsEnd{ false }
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
		while (std::getline(m_iFileStream, m_Line) && !m_IsEnd)
		{
			if (!IsSyntaxValid(m_Line))
				ERROR(GetLastError());

			InterpretLine(m_Line);
			m_IsOperatorFound = false;
			m_LineNumber++;
		}

		if (!m_IsEnd)
			ERROR("<end> statement not found!");
	}

	bool Interpreter::IsSyntaxValid(const std::string& line)
	{
		std::istringstream iss{ line };
		std::string word{};
		while (iss >> word)
		{
			WordType wordType{};
			if (word.front() == '"')
			{
				m_StringValue = word;
				while (m_StringValue.back() != '"' || m_StringValue.size() < 2)
				{
					iss >> word;
					m_StringValue += ' ' + word;
				}
				wordType = GetWordType(m_StringValue);
			}
			else
				wordType = GetWordType(word);

			if (wordType == WordType::Keyword)
			{
				KeywordType keywordType = Keyword::GetKeyword(word);
				if (keywordType == KeywordType::Comment)
					return true;
			}

			if (wordType == WordType::Invalid && word != "=")
			{
				m_ErrorWord = word;
				return false;
			}
		}

		return true;
	}

	void Interpreter::InterpretLine(const std::string& line)
	{
		std::istringstream iss{ line };
		std::string word{};
		iss >> word;

		WordType wordType{};
		if (word.front() == '"')
		{
			m_StringValue = word;
			while (m_StringValue.back() != '"' || m_StringValue.size() < 2)
			{
				iss >> word;
				m_StringValue += ' ' + word;
			}
			wordType = GetWordType(m_StringValue);
		}
		else
			wordType = GetWordType(word);

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
					ERROR("Did you forget the assignment '=' for the variable?");

				// Grab the expression after the '='
				std::string expression = GetExpression(iss);

				// Make sure it's a valid assignment
				ValidateAssignment(varType, expression);

				// Store the variable and its value
				MakeAssignment(varType, variableStr, expression);
				break;
			}

			case WordType::Operator:
				ERROR("'" + word + "' An <operator> is not a valid instruction!");
				break;

			case WordType::Number:
				ERROR("'" + word + "' A <number> is not a valid instruction!");
				break;

			case WordType::String:
				ERROR("'" + word + "' A <string> is not a valid instruction!");
				break;

			case WordType::Invalid:
			default:
				ERROR("'" + word + "' is not supported!");
				break;
		}		
	}

	void Interpreter::Reset()
	{
		m_iFileStream.clear();
		m_Line.clear();
		m_LineNumber = 1;
		m_ErrorWord.clear();

		m_IntValue = 0;
		m_RealValue = 0.0;
		m_StringValue.clear();
		m_IsExponential = false;
		m_IsOperatorFound = false;

		m_IsEnd = false;

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

					//LOG_INFO("Conditional expression: '{0}'", conditionalExpression);
					std::string postfix = InfixToPostfix(conditionalExpression);
					//LOG_INFO("postfix: {0}", postfix);

					bool IsConditionTrue{ false };
					std::istringstream condIss{ conditionalExpression };
					std::string condStr{};
					condIss >> condStr;

					VariableType condVarType = Variable::GetVariableType(condStr);
					switch (condVarType)
					{
						case VariableType::Integer:
							IsConditionTrue = EvaluateOperatorPostfix<int>(postfix);
							break;
						case VariableType::Real:
							IsConditionTrue = EvaluateOperatorPostfix<double>(postfix);
							break;
						case VariableType::String:
							IsConditionTrue = EvaluateStringOperatorPostfix(postfix);
							break;
					}

					if (IsConditionTrue)
					{
						std::string thenStatement{};
						for (std::string tempWord; iss >> tempWord;)
							thenStatement += tempWord + ' ';
						thenStatement.pop_back();

						ValidateKeyword(KeywordType::Then, thenStatement);
					}
				}

				break;
			}

			case KeywordType::Then:
			{
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

					VariableType varType = Variable::GetVariableType(firstWord);
					ValidateAssignment(varType, expr);
					MakeAssignment(varType, firstWord, expr);
				}

				break;
			}

			case KeywordType::Read:
			{
				// Validate that expression is a variable
				if (Variable::IsVariable(expression))
				{
					VariableType varType = Variable::GetVariableType(expression);
					std::string variableStr = expression;
					
					std::string readValue{};
					std::cin >> readValue;

					ValidateAssignment(varType, readValue);
					MakeAssignment(varType, variableStr, readValue);
				}
				else
					ERROR("'" + expression + "' is not a valid assignment!");

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
								ERROR("'" + expression + "' Variable names can not have any digits!");
								return;
							}
						}

						if (Keyword::IsKeyword(expression))
							ERROR("Invalid print statement! Can't print a keyword!");

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
						std::string str{ expression };
						str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
						std::cout << str;
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
				m_IsEnd = true;

				std::cout << '\n';
				LOG_INFO("Program end");
				break;
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
				if (Variable::IsVariable(word))
					break;

				if (!IsInteger(word) || IsReal(word))
					ERROR("Invalid assignment to <integer> variable.");

				break;
			}

			case VariableType::Real:
			{
				if (Variable::IsVariable(word))
					break;

				if (!IsReal(word) || IsInteger(word))
					ERROR("Invalid assignment to <real> variable.");

				break;
			}

			case VariableType::String:
			{
				if (Variable::IsVariable(word))
					break;
				
				if (!IsString(expression))
					ERROR("Invalid assignment to <string> variable.");

				break;
			}
		}

		// Verify arithmetic expressions

		// Grab words until iss is empty
		while (iss >> word)
		{
			if (Operator::IsOperator(word))
			{
				m_IsOperatorFound = true;

				OperatorType op = Operator::GetOperator(word);
				if (Operator::IsLogical(op))
					ERROR(std::string("Used logical operator '" + word + "' in an assignment statement!").c_str());

				if (Operator::IsRelational(op))
					ERROR(std::string("Used relational operator '" + word + "' in an assignment statement!").c_str());

				// Grab next word
				iss >> word;
				if (Operator::IsOperator(word))
					ERROR(std::string("Was expecting a Number or Variable but found '" + word + "'.\nDid you write two operators in a row? ").c_str());
			}
		}
	}

	void Interpreter::PrintVariable(const std::string& expression)
	{
		switch (Variable::GetVariableType(expression))
		{
			case VariableType::Integer:
				if (m_IntHolder.Find(expression))
					std::cout << m_IntHolder.GetValue(expression);
				else
					ERROR("'" + expression + "' is not defined!");
				break;

			case VariableType::Real:
				if (m_RealHolder.Find(expression))
					std::cout << std::fixed << m_RealHolder.GetValue(expression);
				else
					ERROR("'" + expression + "' is not defined!");
				break;

			case VariableType::String:
				if (m_StringHolder.Find(expression))
					std::cout << m_StringHolder.GetValue(expression);
				else
					ERROR("'" + expression + "' is not defined!");
				break;
		}
	}

	void Interpreter::MakeAssignment(VariableType varType, const std::string& variable, const std::string& expression)
	{
		switch (varType)
		{
			case VariableType::Integer:
			{
				if (m_IsOperatorFound)
				{
					std::string postFix = InfixToPostfix(expression);
					int intValue = EvaluateArithmeticPostfix<int>(postFix);
					m_IntHolder.InsertToMap(variable, intValue);
				}
				else
				{
					for (const char& ch : expression)
						if (!isdigit(ch) && ch != '-')
							ERROR("'" + expression + "' is not a valid integer number!");

					int intValue = stoi(expression);
					m_IntHolder.InsertToMap(variable, intValue);
				}
			}
			break;

			case VariableType::Real:
			{
				if (m_IsOperatorFound)
				{
					std::string postFix = InfixToPostfix(expression);
					double realValue = EvaluateArithmeticPostfix<double>(postFix);
					m_RealHolder.InsertToMap(variable, realValue);
				}
				else
				{
					for (const char& ch : expression)
						if (!isdigit(ch) && ch != '.' && ch != '-')
							ERROR("'" + expression + "' is not a valid real number!");

					double realValue = stod(expression);
					m_RealHolder.InsertToMap(variable, realValue);
				}
			}
			break;

			case VariableType::String:
			{
				if (m_IsOperatorFound)
				{
					std::string stringResult = Concatenate(expression);
					stringResult.erase(std::remove(stringResult.begin(), stringResult.end(), '"'), stringResult.end());
					m_StringHolder.InsertToMap(variable, stringResult);
				}
				else
				{
					std::string str{ expression };
					str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
					m_StringHolder.InsertToMap(variable, str);
				}
			}
			break;

			case VariableType::Invalid:
			default:
				ERROR("Cannot assign " + expression + " to " + variable + "!");
				break;
		}
	}

	int Interpreter::Precedence(std::string word)
	{
		if (Operator::IsOperator(word))
		{
			OperatorType op = Operator::GetOperator(word);
			switch (op)
			{
				case OperatorType::Or:
					return 1;
				case OperatorType::And:
					return 2;
				case OperatorType::Not:
					return 3;
				case OperatorType::Eq:
				case OperatorType::Ne:
					return 4;
				case OperatorType::Lt:
				case OperatorType::Le:
				case OperatorType::Gt:
				case OperatorType::Ge:
					return 5;
				case OperatorType::Add:
				case OperatorType::Sub:
					return 6;
				case OperatorType::Mul:
				case OperatorType::Div:
					return 7;
			}
		}
		else
			return -1;
	}

	std::string Interpreter::InfixToPostfix(std::string infix)
	{
		Stack<std::string> stack{};
		std::string result{};

		std::istringstream iss{ infix };
		for (std::string word{}; iss >> word;)
		{
			// If word is an operand
			if (!Operator::IsOperator(word))
				result += word + ' ';

			// If word is an operator
			else
			{
				while (!stack.IsEmpty() && Precedence(word) <= Precedence(stack.Peek()))
				{
					result += stack.Peek() + ' ';
					stack.Pop();
				}
				stack.Push(word);
			}
		}

		while (!stack.IsEmpty())
		{
			result += stack.Peek() + ' ';
			stack.Pop();
		}

		result.pop_back();
		return result;
	}

	bool Interpreter::OperateRelational(OperatorType op, const std::string& operand1, const std::string& operand2)
	{
		switch (op)
		{
			case OperatorType::Eq:
				return operand1 == operand2;
			case OperatorType::Ne:
				return operand1 != operand2;
			case OperatorType::Lt:
				return operand1 < operand2;
			case OperatorType::Le:
				return operand1 <= operand2;
			case OperatorType::Gt:
				return operand1 > operand2;
			case OperatorType::Ge:
				return operand1 >= operand2;
		}

		return false;
	}

	std::string Interpreter::EvaluateStringPostfix(std::string postfix)
	{
		Stack<std::string> stack;
		std::string result{};
		std::string word{};
		std::istringstream iss{ postfix };

		std::string stringValue{};
		while (iss >> word)
		{
			WordType wordType{};

			if (word.front() == '"')
			{
				stringValue = word;
				while (stringValue.back() != '"' || stringValue.size() < 2)
				{
					iss >> word;
					stringValue += ' ' + word;
				}
				wordType = GetWordType(stringValue);
			}
			else
				wordType = GetWordType(word);

			if (!Operator::IsOperator(word))
			{
				switch (wordType)
				{
					case WordType::String:
						stack.Push(stringValue);
						break;

					case WordType::Number:
						stack.Push(word);
						break;

					case WordType::Variable:
					{
						VariableType varType = Variable::GetVariableType(word);
						switch (varType)
						{
							case VariableType::Integer:
								if (m_IntHolder.Find(word))
									stack.Push(std::to_string(m_IntHolder.GetValue(word)));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::Real:
								if (m_RealHolder.Find(word))
									stack.Push(std::to_string(m_RealHolder.GetValue(word)));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::String:
								if (m_StringHolder.Find(word))
									stack.Push(m_StringHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;
						}
					}
					break;
				}
			}
			else
			{
				std::string operand2 = stack.Peek();
				stack.Pop();

				std::string operand1 = stack.Peek();
				stack.Pop();

				result = operand1 + operand2;
				stack.Push(result);
			}
		}

		return result;
	}

	bool Interpreter::EvaluateStringOperatorPostfix(std::string postfix)
	{
		Stack<std::string> stack;
		bool result{ false };
		std::string word{};
		std::istringstream iss{ postfix };

		std::string stringValue{};
		while (iss >> word)
		{
			WordType wordType{};

			if (word.front() == '"')
			{
				stringValue = word;
				while (stringValue.back() != '"' || stringValue.size() < 2)
				{
					iss >> word;
					stringValue += ' ' + word;
				}
				wordType = GetWordType(stringValue);
			}
			else
				wordType = GetWordType(word);

			if (!Operator::IsOperator(word))
			{
				switch (wordType)
				{
					case WordType::String:
					{
						std::string str{ stringValue };
						str.erase(std::remove(str.begin(), str.end(), '"'), str.end());
						stack.Push(str);
						break;
					}

					case WordType::Number:
						stack.Push(word);
						break;

					case WordType::Variable:
					{
						VariableType varType = Variable::GetVariableType(word);
						switch (varType)
						{
							case VariableType::Integer:
								if (m_IntHolder.Find(word))
									stack.Push(std::to_string(m_IntHolder.GetValue(word)));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::Real:
								if (m_RealHolder.Find(word))
									stack.Push(std::to_string(m_RealHolder.GetValue(word)));
								else
									ERROR("'" + word + "' is not defined!");
								break;

							case VariableType::String:
								if (m_StringHolder.Find(word))
									stack.Push(m_StringHolder.GetValue(word));
								else
									ERROR("'" + word + "' is not defined!");
								break;
						}
					}
					break;
				}
			}
			else
			{
				OperatorType op = Operator::GetOperator(word);

				std::string operand2 = stack.Peek();
				stack.Pop();

				std::string operand1 = stack.Peek();
				stack.Pop();

				result = OperateRelational(op, operand1, operand2);
				stack.Push(std::to_string(result));
			}
		}

		return result;
	}

	std::string Interpreter::Concatenate(std::string expression)
	{		
		std::string result{};
		
		std::string postfix = InfixToPostfix(expression);
		result = EvaluateStringPostfix(postfix);

		return result;
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
		if (word == "=") return WordType::Invalid;
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

	std::string Interpreter::GetLastError()
	{
		if (m_ErrorWord.front() == '.' && m_ErrorWord.back() == '.')
			return "'" + m_ErrorWord + "' is an invalid operator! Valid operators are: .add. .sub. .mul. .div. .or. .and. .not. .eq. .ne. .lt. .le. .gt. .ge.";

		for (const char& ch : m_ErrorWord)
			if (isdigit(ch))
				return "'" + m_ErrorWord + "' is not a valid variable! Variables can only be letters a-z or A-Z.";

		return m_ErrorWord;
	}
}
