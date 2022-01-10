#pragma once

#include "Core/VariableHolder.h"

namespace Interpreter
{
	class Interpreter
	{
	public:
		Interpreter();
		virtual ~Interpreter() = default;

		// File management
		bool LoadFile(const std::string& filepath);
		void CloseFile() { m_iFileStream.close(); }

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

		bool IsVariableStored(const std::string& statement);

		bool MakeAssignment(const std::string& variable, const std::string& expression);

		void PrintUserVariable(const std::string& variable);

		Keyword  GetKeyword(std::string statement);
		VariableType GetVariableType(const std::string& statement);

	private:
		// File management
		std::ifstream m_iFileStream;
		std::string m_Line;
		unsigned int m_LineNumber;

		// User variables
		IntegerHolder m_IntegerHolder;
		RealHolder m_RealHolder;
		StringHolder m_StringHolder;

		// Keywords
		std::unordered_map<std::string, Keyword> m_KeywordMap;
	};
}
// resources/sampleLAO.txt