#include "RegularExpression.h"

#include <algorithm>
#include <ranges>

RegularExpression::RegularExpression()
{
	m_pattern = "";
	m_regexPattern = std::regex();
}

RegularExpression::RegularExpression(const string& pattern)
{
	std::regex temp(pattern);
	m_regexPattern = std::move(temp);
	m_pattern= pattern;
}

bool RegularExpression::IsWordAccepted(const string& word) const
{
	return std::regex_match(word, m_regexPattern);
}

RegularExpression RegularExpression::Concatenate(const RegularExpression& other) const
{
	return { m_pattern + other.m_pattern };
}

RegularExpression RegularExpression::KleeneStar() const
{
	if(m_pattern.empty())
	{
		return { m_pattern};
	}
	if(m_pattern.size()==1)
	{
		return { m_pattern + "*" };
	}
	if(m_pattern[0] == '(' && m_pattern[m_pattern.size()-1] == ')')
	{
		return { m_pattern + "*" };
	}
	return { "(" +m_pattern + ")*" };
}

RegularExpression RegularExpression::Union(const RegularExpression& other) const
{
	if(m_pattern.empty())
	{
		return other;
	}
	if(other.m_pattern.empty())
	{
		return *this;
	}
	return { m_pattern + "|" + other.m_pattern };
}

RegularExpression RegularExpression::Plus() const
{
	return { "(" + m_pattern + ")+" };
}

string RegularExpression::GetPattern() const
{
	return m_pattern;
}


bool RegularExpression::operator==(const RegularExpression& other) const
{
		return m_pattern ==other.m_pattern;
}

std::ostream& operator<<(std::ostream& os, const RegularExpression& obj)
{
	os << obj.m_pattern;
	return os;
}

