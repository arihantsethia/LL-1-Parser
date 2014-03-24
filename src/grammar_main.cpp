#include "headers/grammar.h"

int main(){

	std::string filename;
	std::cout<<"Enter the File Name : ";
	std::cin>>filename;

	LLGrammar grammar(filename);
	grammar.getTerminals(true);
	grammar.getNonTerminals(true);
	grammar.getProductionTable(true);
	grammar.computeFollowSets(true);
	grammar.computeFirstSets(true);
	grammar.parseTableConstruction() ;
	return 0;
}