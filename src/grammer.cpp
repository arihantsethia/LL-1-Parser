#include "headers/grammer.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){
}

LLGrammar::~LLGrammar(){

}
 
void LLGrammar::computeEpsilonSets(){

	std::map<std::string,bool>	ischeck ;
	
	ischeck.clear()	  ;
	eInFirsts.clear() ;
	
	for(int i=0 ; i<terminals.size() ; i++){
		if(terminals[i]=="@"){
			eInFirsts[terminals[i]] = true  ;
		}	
		else
			eInFirsts[terminals[i]] = false ;
		ischeck[terminals[i]]   = true  ;
	}

	for(int i=0 ; i<non_terminals.size() ; i++){
		eInFirsts[non_terminals[i]] = false ;
		ischeck[non_terminals[i]]   = false ;  
	}

	
	bool isloop = true ;
	while(isloop){

		for(int i=0; i<non_terminals.size() ;i++){
			
			if(!ischeck[non_terminals[i]]){
				
				bool find = true ;
				int j ;
				for(j=0 ; j< productions[non_terminals[i]].size() ; j++){

					std::vector<std::string> tokens = tokenize(productions[non_terminals[i]][j],".") ;
					
					bool isepsilon = true ;
					for(int k=0	; k<tokens.size() ; k++){
						if(!ischeck[tokens[k]]){
							std::cout << "find = false" << non_terminals[i] <<std::endl ;
							find = false ;
							break	;
						}else if(!eInFirsts[tokens[k]]){
							isepsilon = false ;
						}
					}
					if(!find){
						break ;
					}else if(isepsilon){
						std::cout << "isepsilon" << non_terminals[i] <<std::endl ;

						eInFirsts[non_terminals[i]] = true ;
						ischeck[non_terminals[i]]   = true ;
						break ;
					}

				}

				if(find && j==productions[non_terminals[i]].size()){
					std::cout << "j==prod" << non_terminals[i] <<std::endl ;
					eInFirsts[non_terminals[i]] = false ;
					ischeck[non_terminals[i]]   = true  ;
				}

			}
		}


		isloop = false ;
		for(int i=0; i<non_terminals.size() ;i++){
			if(!ischeck[non_terminals[i]]){
				isloop = true ;
			}
		}
	}
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
	return eInFirsts[symbol] ;
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