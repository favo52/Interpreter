#pragma once

namespace Interpreter
{
	struct Token
	{
		char Kind;
		double Value{ 0.0 };
		std::string Name;

		Token(char ch) : Kind{ ch } {}
		Token(char ch, double val) : Kind{ ch }, Value{ val } {}
		Token(char ch, std::string name) : Kind{ ch }, Name{ name } {}
	};

	class TokenStream
	{
	public:
		TokenStream() = default;

		const Token& Get();

		void Putback(const Token& token);
		void Ignore(char ch);

	private:
		bool m_Full{ false };
		Token m_Buffer{ 0 };
	};
}
