#include "pch.h"
#include "Token.h"

namespace Interpreter
{
	constexpr char PRINT{ ';' };

	const Token& TokenStream::Get()
	{
		// Check if we already have a Token ready
		if (m_Full)
		{
			m_Full = false;
			return m_Buffer;
		}

		char ch;
		std::cin.get(ch);
		while (isspace(ch))
		{
			if (ch == '\n') return Token(PRINT);
			std::cin.get(ch);
		}

		switch (ch)
		{
			case '.': // operator
				break;

			default:
				break;
		}
	}

	void TokenStream::Putback(const Token& token)
	{
		m_Buffer = token;
		m_Full = true;
	}

	void TokenStream::Ignore(char c)
	{
		// Look in Buffer
		if (m_Full && c == m_Buffer.Kind)
		{
			m_Full = false;
			return;
		}
		m_Full = false;

		// Search input
		char ch;
		while (std::cin >> ch)
			if (ch == c) return;
	}
}
