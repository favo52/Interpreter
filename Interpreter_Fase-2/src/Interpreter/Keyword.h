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
		Keyword();
		virtual ~Keyword() = default;

		static bool IsKeyword(std::string word);

		static void InterpretKeyword(KeywordType keyword);

		static KeywordType GetKeyword(std::string word);

	private:
		static std::unordered_map<std::string, KeywordType> m_KeywordMap;
	};
}
