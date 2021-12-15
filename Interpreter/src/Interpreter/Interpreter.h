#pragma once

namespace Interpreter
{
	class Interpreter
	{
	public:
		Interpreter();
		virtual ~Interpreter() = default;

		// File management
		void LoadFile(const std::string& filepath);
		inline void CloseFile() { m_iFileStream.close(); }

		void ReadFile();

	private:
		enum class Keyword
		{
			Invalid = 0,
			Comment,
			If,
			Then,
			Read,
			Print,
			End
		};

		enum class VariableType
		{
			Invalid = 0,
			Integer,
			Real,
			String
		};

	private:
		bool IsVariable(const std::string& str);
		bool IsKeyword(std::string statement);

		bool MakeAssignment(const std::string& variable, const std::string& expression);

		Keyword  GetKeyword(std::string statement);
		VariableType GetVariableType(const std::string& statement);

	private:
		// File management
		std::ifstream m_iFileStream;
		std::string m_Line;
		unsigned int m_LineNumber;

		// User variables
		std::vector<int> m_Integers;
		std::vector<double> m_Reals;
		std::vector<std::string> m_Strings;

		// Keywords
		std::unordered_map<std::string, Keyword> m_KeywordMap;
	};
}
