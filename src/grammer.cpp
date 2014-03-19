#include "headers/grammer.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){

}

LLGrammar::~LLGrammar(){

}
 
void LLGrammar::computeEpsilonSets(){

}

void LLGrammar::computeFirst(std::string symbol){

}

void LLGrammar::computeFollow(std::string symbol){

}

void LLGrammar::computeFirstSets(){

}

void LLGrammar::computeFollowSets(){

}

std::set<std::string> LLGrammar::getFirst(std::string symbol){

}

std::set<std::string> LLGrammar::getFollow(std::string symbol){

}

bool LLGrammar::containsEpsilon(std::string symbol){
	
	return true;
}

// This function tokenizes the string on the basis of delimeters space or newline or cariage return.
std::vector<std::string> tokenize(std::string s, std::string sep){
	// Skip delimiters at beginning.
	std::string::size_type lastPos = s.find_first_not_of(sep, 0);	
	// Find first "non-delimiter", which will be between lastPos and pos
	std::string::size_type pos = s.find_first_of(sep, lastPos); 
	std::vector<std::string> tokens;
	while(pos != std::string::npos || lastPos != std::string::npos){
		tokens.push_back(s.substr(lastPos,(pos - lastPos)));
		// Skip delimiters
		lastPos = s.find_first_not_of(sep, pos);	
		// Find "non-delimiter", which will be between lastPos and pos
		pos = s.find_first_of(sep, lastPos); 
	}
	return tokens;
}