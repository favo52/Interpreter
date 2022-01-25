#pragma once

namespace Interpreter
{
	enum class KeywordType
	{
		Invalid = 0,
		Comment, If, Then, Read, Print, End
	};

	class Keyword
	{
	public:
		static bool IsKeyword(std::string word);
		static KeywordType GetKeyword(std::string word);

		static void ValidateKeyword(KeywordType keywordType, std::string expression);

	private:
		static std::unordered_map<std::string, KeywordType> m_KeywordMap;
	};
}
