#include "headers/grammer.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){
	std::set<std::string> setTerminals,setNonTerminals,symbolsSet;
	std::vector<std::string> _productions, _symbols;
	std::string line,nonterminal;
	std::ifstream in(filename.c_str());
	while(getline(in,line)){
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

std::set<std::string> LLGrammar::computeFirst(std::string symbol){
	std::set<std::string> firstSet, anotherFirstSet;
	std::vector<std::string> _symbols;
	int flag=0;

	if(containsEpsilon(symbol)){
		firstSet.insert("@");
	}

	for(int i=0 ; i<productions[symbol].size(); i++){
		std::vector<std::string> _symbols = tokenize(productions[symbol][i],".");
		for(int j = 0; j<_symbols.size(); j++){
			if(!(containsEpsilon(_symbols[j]))){
				flag=1;
				if(find(terminals.begin(), terminals.end(), _symbols[j]) != terminals.end()){
					firstSet.insert(_symbols[j]);
				}
				else{
					anotherFirstSet=computeFirst(_symbols[j]);
					std::copy(anotherFirstSet.begin(), anotherFirstSet.end(), std::inserter(firstSet, firstSet.end()));
				}
				break;
			}
		}
		if (flag==0){
			firstSet.insert("@");
		}
	}

	return firstSet;
}

std::set<std::string> LLGrammar::computeFollow(std::string symbol){

}

void LLGrammar::computeFirstSets(){
	for(int i = 0; i<non_terminals.size(); i++){
		firstSets[non_terminals[i]]=computeFirst(non_terminals[i]);
	}

	for(int i = 0; i<terminals.size(); i++){
		firstSets[terminals[i]].clear();
	}

	std::cout<<"First Set is  : "<<std::endl;
	std::map<std::string, std::set<std::string> >::iterator it;
	std::set<std::string>::iterator itSet;
	for(it=firstSets.begin();it!=firstSets.end();it++){
		std::cout<<(*it).first<<" -> ";
		for(itSet=(*it).second.begin();itSet != (*it).second.end();itSet++){
			std::cout<<(*itSet)<<" | ";
		}
		std::cout<<std::endl;
	}
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