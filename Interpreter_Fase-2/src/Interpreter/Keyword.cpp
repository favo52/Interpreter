#include "pch.h"
#include "Keyword.h"

namespace Interpreter
{
	std::unordered_map<std::string, KeywordType> Keyword::m_KeywordMap{};

	Keyword::Keyword()
	{
		m_KeywordMap =
		{
			{"REM", KeywordType::Comment},
			{"IF", KeywordType::If},
			{"THEN", KeywordType::Then},
			{"READ", KeywordType::Read},
			{"PRINT", KeywordType::Print},
			{"END.", KeywordType::End}
		};
	}

	bool Keyword::IsKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search hash map for the KeywordType
		auto x = m_KeywordMap.find(word);
		if (x != std::end(m_KeywordMap))
			return true;
		else
			return false;
	}

	void Keyword::InterpretKeyword(KeywordType keywordType)
	{
		switch (keywordType)
		{
			case KeywordType::Comment:
			{
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
				break;
			}

			case KeywordType::Print:
			{
				break;
			}

			case KeywordType::End:
			{
				break;
			}

			case KeywordType::Invalid: LOG_ERROR("<invalid KeywordType>"); break;

			default:
				LOG_ERROR("Unknown KeywordType!"); break;
		}
	}

	KeywordType Keyword::GetKeyword(std::string word)
	{
		// Make all characters uppercase
		for (char& c : word)
			c = std::toupper(c);

		// Search hash map for the Keywords
		auto x = m_KeywordMap.find(word);
		if (x != std::end(m_KeywordMap))
			return x->second;
		else
			return KeywordType::Invalid;
	}
}
