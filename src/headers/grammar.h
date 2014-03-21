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
#include <sstream>
#include <fstream>
#include <utility>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <cstring>
#include <list>

class LLGrammar{
private:
	std::string startSymbol;
	std::vector<std::string> terminals;
	std::vector<std::string> nonTerminals;
	std::map<std::string, std::set<std::string> > firstSets;
	std::map<std::string, std::set<std::string> > followSets;
	std::map<std::string, std::vector<std::string> > productions;
	std::map<std::string, bool> eInFirsts;

	std::string getNextNonTerminalSymbol(std::string);
	std::vector<std::string> tokenize(std::string, std::string);
	std::set<std::string> computeFirst(std::string);
	void computeEpsilonSets();	
	void computeFollow(std::string,std::map<std::string, std::vector<std::string> >&);
	bool removeLeftRecursion();
	bool leftFactor();
	void optimizeTable();
	std::vector<std::string> removeDuplicates(std::vector<std::string>);
	std::string removeExtraEplisons(std::string);

public:

	LLGrammar();
	LLGrammar(std::string);
	~LLGrammar();
	void computeFirstSets(bool print=false);
	void computeFollowSets(bool print=false);
	std::set<std::string> getFirst(std::string);
	std::set<std::string> getFollow(std::string);
	bool containsEpsilon(std::string);
	std::vector<std::string> getTerminals(bool print=false);
	std::vector<std::string> getNonTerminals(bool print=false);
	std::map<std::string, std::vector<std::string> > getProductionTable(bool print=false);
	void parseTableConstruction() ;
};

#endif