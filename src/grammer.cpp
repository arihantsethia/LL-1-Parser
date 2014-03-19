#include "headers/grammer.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){
	std::set<std::string> setTerminals,setNonTerminals,symbolsSet;
	std::vector<std::string> _productions, _symbols;
	std::string line,nonterminal;
	std::ifstream in(filename.c_str());
	while(getline(in,line)){
		std::cout<<line<<std::endl;
		std::string::size_type beginPos = line.find_first_not_of(" ");
		if(beginPos!= std::string::npos){
			std::string::size_type endPos = line.find("->",beginPos);
			if( endPos != std::string::npos){
				nonterminal = line.substr(beginPos,endPos-beginPos);
				// To remove spaces from nonterminal if any.
				std::cout<<nonterminal<<std::endl;
				beginPos = nonterminal.find_first_of(" ",0);
				if(beginPos != std::string::npos){
					nonterminal = nonterminal.substr(0,beginPos);
				}				
				setNonTerminals.insert(nonterminal);
				line = line.substr(endPos+2);
				_productions = tokenize(line," |");
				for(int i=0;i<_productions.size();i++){
					productions[nonterminal].push_back(_productions[i]);
					_symbols = tokenize(_productions[i],".");
					std::copy(_symbols.begin(), _symbols.end(), std::inserter(symbolsSet, symbolsSet.end()));
				}
			}else{
				std::cout<<"Error \n";
				return;
			}
		}
	}
	std::copy(setNonTerminals.begin(), setNonTerminals.end(), std::back_inserter(non_terminals));
	if(symbolsSet.find("@")!=symbolsSet.end()){		
		symbolsSet.erase(symbolsSet.find("@"));
	}
	std::set_difference (symbolsSet.begin(), symbolsSet.end(), setNonTerminals.begin(), setNonTerminals.end(), std::back_inserter(terminals));
	in.close();
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
std::vector<std::string> LLGrammar::tokenize(std::string s, std::string sep){
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

std::vector<std::string> LLGrammar::getTerminals(bool print){
	if(print){
		std::cout<<"List of terminals are : "<<std::endl;
		for(int i=0;i<terminals.size();i++){
			std::cout<<i+1<<")\t"<<terminals[i]<<std::endl;
		}
	}
	return terminals;
}

std::vector<std::string> LLGrammar::getNonTerminals(bool print){
	if(print){
		std::cout<<"List of non-terminals are : "<<std::endl;
		for(int i=0;i<non_terminals.size();i++){
			std::cout<<i+1<<")\t"<<non_terminals[i]<<std::endl;
		}
	}
	return non_terminals;
}

std::map<std::string, std::vector<std::string> > LLGrammar::getProductionTable(bool print){
	if(print){
		std::cout<<"Production Table is  : "<<std::endl;
		std::map<std::string, std::vector<std::string> >::iterator it;
		for(it=productions.begin();it!=productions.end();it++){
			std::cout<<(*it).first<<" -> ";
			for(int i=0;i<(*it).second.size();i++){
				std::cout<<(*it).second[i]<<" | ";
			}
			std::cout<<std::endl;
		}		
	}
	return productions;
}