# This program transforms a nondeterministic automaton into a regular expression.



# Input Format:

The first line consists of the states of the automaton separated by whitespace.
The second line contains the input alphabet separated by whitespace. (Note: tested only on symbols one character long)
The third line indicates the starting state.
The fourth line specifies the finishing states (there can be more than one).
From the fifth line onwards, each line represents a transition with the following format:
The source state.
The symbol for transitioning between states.
The destination state.



# Example Input:
q1 q2
a b c
q1
q2
q1 a q1
q1 b q2
q2 c q2
q2 b q1