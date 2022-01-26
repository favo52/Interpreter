#include "pch.h"
#include "Keyword.h"

namespace Interpreter
{
	std::unordered_map<std::string, KeywordType> Keyword::m_KeywordMap =
	{
		{"REM", KeywordType::Comment},
		{"IF", KeywordType::If},
		{"THEN", KeywordType::Then},
		{"READ", KeywordType::Read},
		{"PRINT", KeywordType::Print},
		{"END.", KeywordType::End}
	};

	bool Keyword::IsKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search for the string in the hash map
		auto x = m_KeywordMap.find(word);
		if (x != m_KeywordMap.end())
			return true;
		else
			return false;
	}

	KeywordType Keyword::GetKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search hash map for the KeywordType
		auto x = m_KeywordMap.find(word);
		if (x != m_KeywordMap.end())
			return x->second;
		else
			return KeywordType::Invalid;
	}

	void Keyword::ValidateKeyword(KeywordType keywordType, std::string expression)
	{
		switch (keywordType)
		{
			case KeywordType::Comment:
			{
				LOG_INFO("<comment statement> '{0}'", expression);
				break;
			}

			case KeywordType::If:
			{
				break;
			}

			case KeywordType::Then:
			{
				break;
			}

			case KeywordType::Read:
			{
				LOG_TRACE("<read statement>");
				break;
			}

			case KeywordType::Print:
			{
				LOG_TRACE("<print statement>'{0}'", expression);
				break;
			}

			case KeywordType::End:
			{
				if (!expression.empty())
				{
					LOG_ERROR("Error, END. is not alone.");
					return;
				}

				LOG_TRACE("<end statement>");
				break;
			}

			case KeywordType::Invalid: LOG_ERROR("<invalid KeywordType>"); break;

			default:
				LOG_ERROR("Unknown KeywordType!"); break;
		}
	}
}
