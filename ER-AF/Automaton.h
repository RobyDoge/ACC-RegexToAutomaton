﻿#pragma once
#include <map>
#include <set>
#include <string>
#include "RegularExpression.h"


class Automaton
{
public:
	void CombineSimilarTransitions();
	Automaton(const string& fileName);
	RegularExpression GetRegularExpression();

private:
	void CreateNewInitialAndFinalStates();
	void CreateNewTransitions(const string& stateToEliminate, const std::map<std::string, std::string>& connectedStates);
	void EraseState(const string& stateToEliminate);
	void EraseNextState();
	RegularExpression FindRegExTransition(const string& fromState, const string& toState) const;
	std::map<string, string> FindConnectedStates(const string& bridgeState) const;

private:
	std::set<string> m_states;
	std::set<char> m_alphabet;
	string m_startState;
	std::set<string> m_finalStates;
	//q0 -> {a,q1}
	std::multimap<string,std::pair<RegularExpression,string>> m_transitions;

};
