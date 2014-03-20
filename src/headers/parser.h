#ifndef PARSER_H
#define PARSER_H

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

class LLParser{
private:
	std::map< std::pair<std::string,std::string> , std::string > parseTable;
	std::vector<std::string> tokens;

	void createParseTable();
	std::vector<std::string> tokenize(std::string, std::string);

public:
	LLParser();
	~LLParser();
	LLParser(std::vector<std::string> _tokens);
	bool parse(bool print=true);
};
#endif