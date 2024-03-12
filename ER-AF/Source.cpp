#include <iostream>
#include "Automaton.h"

int main()
{
	Automaton automaton("input.txt");
	auto regExp = automaton.GetRegularExpression();
	std::cout << "Regular expression: " << regExp << '\n';

	return 0;
}