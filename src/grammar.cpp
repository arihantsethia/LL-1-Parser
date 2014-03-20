#include "headers/grammar.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){
	firstSets.clear();
	eInFirsts.clear();
	startSymbol = "";
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
				beginPos = nonterminal.find_first_of(" ",0);
				if(beginPos != std::string::npos){
					nonterminal = nonterminal.substr(0,beginPos);
				}
				// To store the starting symbol of the language.
				if(setNonTerminals.empty()){
					startSymbol = nonterminal;
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
	std::copy(setNonTerminals.begin(), setNonTerminals.end(), std::back_inserter(nonTerminals));
	if(symbolsSet.find("@")!=symbolsSet.end()){	
		symbolsSet.erase(symbolsSet.find("@"));
	}
	std::set_difference (symbolsSet.begin(), symbolsSet.end(), setNonTerminals.begin(), setNonTerminals.end(), std::back_inserter(terminals));
	in.close();
}

LLGrammar::~LLGrammar(){

}
 
void LLGrammar::computeEpsilonSets(){

	std::map<std::string,bool>	isChecked ;
	std::vector<std::string> tokens;
	bool flag1, flag2, isEpsilon,continueInLoop = true;
	isChecked.clear();
	
	for(int i=0 ; i<terminals.size() ; i++){
		eInFirsts[terminals[i]] = false;
		isChecked[terminals[i]] = true;
	}

	for(int i=0 ; i<nonTerminals.size() ; i++){
		eInFirsts[nonTerminals[i]] = false ;
		isChecked[nonTerminals[i]] = false ;  
	}
	
	while(continueInLoop){
		for(int i=0; i<nonTerminals.size() ;i++){
			if(!isChecked[nonTerminals[i]]){
				flag1 = true ;
				for(int j=0 ; j< productions[nonTerminals[i]].size() ; j++){
					tokens = tokenize(productions[nonTerminals[i]][j],".") ;
					flag2 = true ;
					isEpsilon = true ;
					for(int k=0	; k<tokens.size() ; k++){
						if(tokens[k]!="@"){
							if(!eInFirsts[tokens[k]])
							{
								isEpsilon = false ;
								if(isChecked[tokens[k]]){
									flag2 = false ;
								}
							}
						}
					}

					if(!flag2 && flag1){
						flag1 = true ;
					}
					else {
						flag1 = false ;
					}

					if(isEpsilon){
						eInFirsts[nonTerminals[i]] = true ;
						isChecked[nonTerminals[i]]   = true ;
						break ;
					}
				}
				if(flag1){
					eInFirsts[nonTerminals[i]] = false ;
					isChecked[nonTerminals[i]]   = true  ;
				}
			}
			continueInLoop = false ;
			for(int i=0; i<nonTerminals.size() ;i++){
				if(!isChecked[nonTerminals[i]]){
					continueInLoop = true ;
				}
			}
		}
	}
}

std::set<std::string> LLGrammar::computeFirst(std::string symbol){

	if(firstSets.find(symbol) != firstSets.end()){
		return firstSets[symbol];
	}
	std::set<std::string> firstSet, anotherFirstSet;
	std::vector<std::string> _symbols;
	int flag=0;

	if(containsEpsilon(symbol)){
		firstSet.insert("@");
	}

	for(int i=0 ; i<productions[symbol].size(); i++){
		_symbols = tokenize(productions[symbol][i],".");
		for(int j = 0; j<_symbols.size(); j++){
			if(!(containsEpsilon(_symbols[j]))){
				flag=1;
				if(find(terminals.begin(), terminals.end(),_symbols[j]) != terminals.end()){
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

void LLGrammar::computeFollow(std::string symbol,std::map<std::string, std::vector<std::string> >& tempFollowSets ){
//	std::set<std::string> followSet, anotherFollowSet;
	std::vector<std::string> _symbols;
	if(symbol == startSymbol){
		tempFollowSets[symbol].push_back("$");
	}

	for(int i=0;i<productions[symbol].size();i++){
		_symbols = tokenize(productions[symbol][i],".");
		for(int j=0; j< _symbols.size()-1;j++){
			if(_symbols[j]!="@"){
				tempFollowSets[_symbols[j]].push_back("FIRST_"+_symbols[j+1]);
			}
		}
		for(int j=_symbols.size(); j>0;j--){
			if(j==_symbols.size() || containsEpsilon(_symbols[j])){
				if(_symbols[j-1]!="@"){
					tempFollowSets[_symbols[j-1]].push_back("FOLLOW_"+symbol);
				}
			}else{
				break;
			}
		}
	}
}

void LLGrammar::computeFirstSets(){

	computeEpsilonSets();

	for(int i = 0; i<nonTerminals.size(); i++){
		if(firstSets.find(nonTerminals[i]) == firstSets.end()){
			firstSets[nonTerminals[i]]=computeFirst(nonTerminals[i]);
		}
	}

	for(int i = 0; i<terminals.size(); i++){
		firstSets[terminals[i]].clear();
		firstSets[terminals[i]].insert(terminals[i]);
	}
	firstSets["@"].insert("@");
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
	std::map<std::string, std::vector<std::string> > tempFollowSets;
	std::map<std::string, std::vector<std::string> >::iterator it;
	std::vector<std::string> tempVector, bfsVector;
	std::string symbol, _symbol; 
	for(int i=0;i<nonTerminals.size();i++){
		computeFollow(nonTerminals[i],tempFollowSets);
	}

	for(it=tempFollowSets.begin();it!=tempFollowSets.end();it++){
		tempVector = (*it).second;
		std::unique (tempVector.begin(), tempVector.end());
		tempVector.erase(std::remove(tempVector.begin(), tempVector.end(), "@"), tempVector.end());
	}
	
	for(it = tempFollowSets.begin(); it!=tempFollowSets.end(); it++ ){
		tempVector = (*it).second;
		std::map<std::string,bool> visited;
		std::queue<std::string> q;
		visited[(*it).first]=true;
		for(int i=0; i < tempVector.size(); i++){
			if(tempVector[i] == "$"){
				followSets[(*it).first].insert("$");
			}
			if(tempVector[i].find("FIRST_") == 0 ){
				symbol = tempVector[i].substr(6);
				std::copy(firstSets[symbol].begin(), firstSets[symbol].end(), std::inserter(followSets[(*it).first], followSets[(*it).first].end()));
			}else if ( tempVector[i].find("FOLLOW_") == 0 ){
				symbol = tempVector[i].substr(7);
				// Implementing BFS.
				q.push(symbol);
				while(!q.empty()){
					symbol = q.front();
					q.pop();
					if(visited.find(symbol) != visited.end()){
						continue;
					}
					visited[symbol] = true;
					bfsVector = tempFollowSets[symbol];
					for(int j=0;j<tempFollowSets[symbol].size(); j++){
						if(visited.find(symbol) != visited.end()){
							if(tempFollowSets[symbol][j] == "$"){
								followSets[(*it).first].insert("$");
							}
							if(tempFollowSets[symbol][j].find("FIRST_") == 0){
								_symbol = tempFollowSets[symbol][j].substr(6);
								std::copy(firstSets[_symbol].begin(), firstSets[_symbol].end(), std::inserter(followSets[(*it).first], followSets[(*it).first].end()));
							}else if (tempFollowSets[symbol][j].find("FOLLOW_") == 0){
								_symbol = tempFollowSets[symbol][j].substr(7);
								q.push(_symbol);
							}
						}
					}
				}
			}
		}
	}
	std::cout<<"Follow Set is  : "<<std::endl;
	std::map<std::string, std::set<std::string> >::iterator itFollowSets;
	for(itFollowSets=followSets.begin();itFollowSets!=followSets.end();itFollowSets++){
		if((*itFollowSets).second.find("@")!=(*itFollowSets).second.end()){	
			(*itFollowSets).second.erase((*itFollowSets).second.find("@"));
		}
		std::cout<<(*itFollowSets).first<<" -> ";
		for(std::set<std::string>::iterator itSet=(*itFollowSets).second.begin();itSet != (*itFollowSets).second.end();itSet++){
			std::cout<<(*itSet)<<" | ";
		}
		std::cout<<std::endl;
	}
}

std::set<std::string> LLGrammar::getFirst(std::string symbol){
	std::set<std::string> result;
	if(firstSets.find(symbol)!= firstSets.end())
		result = firstSets[symbol];
	return result; 
}

std::set<std::string> LLGrammar::getFollow(std::string symbol){
	std::set<std::string> result;
	if(followSets.find(symbol)!= followSets.end())
		result = followSets[symbol];
	return result; 
}

bool LLGrammar::containsEpsilon(std::string symbol){
	if(symbol=="@")
		return true ;
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
		for(int i=0;i<nonTerminals.size();i++){
			std::cout<<i+1<<")\t"<<nonTerminals[i]<<std::endl;
		}
	}
	return nonTerminals;
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


void LLGrammar::parseTableConstruction(){

	std::set<std::string> firstSet;
	std::set<std::string> followSet ;
	std::set<std::string>::iterator setIterator ;
	std::map< std::pair<std::string ,std::string >, std::string > parseTable ;
	std::pair<std::string,std::string> tempPair ;
	std::vector<std::string> tokens;
	std::ofstream out("parse_table.txt");
	bool isEpsilon ;

	for(int i=0 ; i<nonTerminals.size() ; i++){
		for(int j=0 ; j<terminals.size() ;j++){
			tempPair = std::make_pair(nonTerminals[i],terminals[j]);
			parseTable[tempPair].assign("error") ;
		}
		tempPair = std::make_pair(nonTerminals[i],"$");
		parseTable[tempPair].assign("error") ;
		
		for(int j=0 ; j< productions[nonTerminals[i]].size() ; j++){
			tokens = tokenize(productions[nonTerminals[i]][j],".") ;
			int k = 0 ;
			isEpsilon = true ;
			while(k < tokens.size() && isEpsilon){
				firstSet = getFirst(tokens[k]);
				for(setIterator=firstSet.begin() ; setIterator!=firstSet.end() ; setIterator++){
					tempPair = std::make_pair(nonTerminals[i],*setIterator);
					parseTable[tempPair].assign(productions[nonTerminals[i]][j]) ;
				}
				if(!containsEpsilon(tokens[k])){
					isEpsilon = false ;
				}
				k++ ;
			}

			if(isEpsilon){
				followSet = getFollow(nonTerminals[i]) ;
				for(setIterator=followSet.begin() ; setIterator!=followSet.end() ; setIterator++){
					tempPair = std::make_pair(nonTerminals[i],*setIterator);
					parseTable[tempPair].assign(productions[nonTerminals[i]][j]) ;				
				}
			}
		}
	}

	out << "\\\t" ;
	for(int j=0 ; j<terminals.size() ;j++){
		out << terminals[j] << "\t" ;
	}
	out << "$\t" << std::endl ;

	for(int i=0 ; i<nonTerminals.size() ; i++){
		out << nonTerminals[i] << "\t" ;
		for(int j=0 ; j<terminals.size() ;j++){
			tempPair = std::make_pair(nonTerminals[i],terminals[j]);
			out << parseTable[tempPair] << "\t" ;
		}
		tempPair = std::make_pair(nonTerminals[i],"$");
		out << parseTable[tempPair] << "\t" << std::endl ;
	}
	out.close() ;
}