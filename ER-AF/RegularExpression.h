#pragma once
#include <ostream>
#include <string>
#include <regex>
using std::string;

class RegularExpression
{
public:
	RegularExpression();
	RegularExpression(const string& pattern);
	bool IsWordAccepted(const string& word) const;
	RegularExpression Concatenate(const RegularExpression& other) const;
	RegularExpression KleeneStar() const;
	RegularExpression Union(const RegularExpression& other) const;
	RegularExpression Plus() const;
	string GetPattern() const;
	bool operator==(const RegularExpression& other) const;
	friend std::ostream& operator<<(std::ostream& os, const RegularExpression& obj);
	

private:
	std::regex m_regexPattern{};
	string m_pattern{};
	
	

};
