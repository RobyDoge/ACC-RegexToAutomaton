#include "Automaton.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ranges>

Automaton::Automaton(const string& fileName)
{
	std::ifstream file(fileName);
	if (!file.is_open())
	{
		std::cerr << "File not found\n";
		return;
	}
	std::string line;
	std::string readChars{};
	std::istringstream iss;

	//Read states
	std::getline(file, line);
	iss.str(line);
	readChars.clear();
	while(iss >> readChars)
	{
		m_states.insert(readChars);
	}

	//Read alphabet
	std::getline(file, line);
	iss.clear();
	iss.str(line);
	readChars.clear();
	while(iss >> readChars)
	{
		m_alphabet.insert(readChars[0]);
	}

	//Read start state
	std::getline(file, line);
	m_startState = line;

	//Read final states
	std::getline(file, line);
	iss.clear();
	iss.str(line);
	readChars.clear();
	while(iss >> readChars)
	{
		m_finalStates.insert(readChars);
	}

	//Read transitions
	while(std::getline(file, line))
	{
		iss.clear();
		iss.str(line);
		string fromState;
		string toState;
		string symbol;
		iss >> fromState >> symbol >> toState;
		m_transitions.insert({fromState,{RegularExpression(symbol),toState}});
	}
	file.close();
}


RegularExpression Automaton::GetRegularExpression()
{
	CreateNewInitialAndFinalStates();
	while(m_states.size() > 2)
	{
		EraseNextState();
	}
	return m_transitions.begin()->second.first;
}

void Automaton::CreateNewInitialAndFinalStates()
{
	const string newStartState = "I";
	const string newFinalState = "F";

	m_states.insert(newStartState);
	m_states.insert(newFinalState);

	m_transitions.insert({ newStartState,{RegularExpression(""),m_startState}});
	std::ranges::for_each(m_finalStates, [this,&newFinalState](const string& finalState)
	{
		m_transitions.insert({finalState,{RegularExpression(""),newFinalState}});
	});
	m_startState= newStartState;
	m_finalStates.clear();
	m_finalStates.insert(newFinalState);
}



void Automaton::EraseNextState()
{
    auto it = m_states.begin();
    std::advance(it, 2);
    const string stateToEliminate = *it;
    const auto connectedStates = FindConnectedStates(stateToEliminate);
    CreateNewTransitions(stateToEliminate, connectedStates);

    // Erase transitions with stateToEliminate as the key
	EraseState(stateToEliminate);
    
}



std::map<string,string> Automaton::FindConnectedStates(const string& bridgeState) const
{
	std::map<string,string> connectedStates;

	for(const auto& [fromState, symbol_toState] : m_transitions)
	{
		if(const auto& [symbol, toState] = symbol_toState; 
			toState == bridgeState)
		{
			auto [rangeBegin, rangeEnd] = m_transitions.equal_range(toState);
			for (auto it = rangeBegin; it != rangeEnd; ++it)
			{
				auto& [rangeSymbol, rangeToState] = it->second;
				if(rangeToState != bridgeState && fromState != bridgeState)
					connectedStates.insert({ fromState,rangeToState });
			}
		}
	}

	return connectedStates;
}

void Automaton::CreateNewTransitions(const string& stateToEliminate,
	const std::map<std::string, std::string>& connectedStates)
{
	std::ranges::for_each(connectedStates, [this, &stateToEliminate](const auto& pair)
		{
			auto [fromState, toState] = pair;
			auto pq = FindRegExTransition(fromState, toState);
			auto pk = FindRegExTransition(fromState, stateToEliminate);
			auto kk = FindRegExTransition(stateToEliminate, stateToEliminate);
			auto qk = FindRegExTransition(stateToEliminate, toState);

			bool directConnectionExists = true;
			if(pq == RegularExpression(""))
				directConnectionExists=false;

			kk = kk.KleeneStar();
			pk = pk.Concatenate(kk);
			pk = pk.Concatenate(qk);
			auto newTransition = pq.Union(pk);

			if (directConnectionExists)
			{
				auto find = std::ranges::find_if(m_transitions, [&fromState, &toState](const auto& pair)
				{
					auto [transitionSymbols, state] = pair.second;
					return fromState == pair.first && toState==state;
				});
				m_transitions.erase(find);
			}
			m_transitions.insert({ fromState,{newTransition,toState} });
		});
}

void Automaton::EraseState(const string& stateToEliminate)
{
	for (auto it = m_transitions.begin(); it != m_transitions.end();)
	{
		if (it->first == stateToEliminate || it->second.second == stateToEliminate)
		{
			it = m_transitions.erase(it);
		}
		else
		{
			++it;
		}
	}
	m_states.erase(stateToEliminate);
}

RegularExpression Automaton::FindRegExTransition(const string& fromState, const string& toState) const
{
	std::vector<RegularExpression> similarTransitions;
	auto [rangeBegin, rangeEnd] = m_transitions.equal_range(fromState);
	for (auto it = rangeBegin; it != rangeEnd; ++it)
	{
		auto& [transitionSymbols, state] = it->second;
		if (state == toState)
		{
			similarTransitions.emplace_back(transitionSymbols);
		}
	}
	if(similarTransitions.empty())
	{
		return {};
	}
	if(similarTransitions.size() == 1)
	{
		return similarTransitions[0];
	}

	RegularExpression newTransition;
	for(const auto& transition : similarTransitions)
	{
		newTransition = newTransition.Union(transition);
	}
	return {"("+newTransition.GetPattern() + ")"};
}
