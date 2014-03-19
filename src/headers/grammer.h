#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <iostream>
#include <string>
#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <utility>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cstring>
#include <list>

class LLGrammar{
private:
	std::vector<std::string> terminals;
	std::vector<std::string> non_terminals;
	std::map<std::string, std::set > firstSets;
	std::map<std::string, std::set > followSets;
	std::map<std::string, std::vector<std::string> > productions;
	std::map<std::string, bool> eInFirsts;

	void computeEpsilonSets();
	void computeFirst(std::string);
	void computeFollow(std::string);

public:
	
	LLGrammar();
	LLGrammer(std::string);
	~LLGrammar();
	computeFirstSets();
	computeFollowSets();
	std::set getFirst(std::string);
	std::set getFollow(std::string);
	bool containsEpsilon(std::string);
};

#endif