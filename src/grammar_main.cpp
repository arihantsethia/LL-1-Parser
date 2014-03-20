#include "headers/grammar.h"

int main(){

	std::string filename;
	std::cout<<"Enter the File Name : ";
	std::cin>>filename;

	LLGrammar grammar(filename);
	grammar.getTerminals(true);
	grammar.getNonTerminals(true);
	grammar.getProductionTable(true);
	grammar.computeFirstSets();
	grammar.computeFollowSets();
	grammar.parseTableConstruction() ;
	return 0;
}