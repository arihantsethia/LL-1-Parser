#include "headers/grammar.h"

LLGrammar::LLGrammar(){

}

LLGrammar::LLGrammar(std::string filename){
	isLL1 = true;
	firstSetsComputed= false;
	followSetsComputed = false;
	firstSets.clear();
	eInFirsts.clear();
	startSymbol = "";
	std::set<std::string> setTerminals,setNonTerminals,symbolsSet;
	std::vector<std::string> _productions, _symbols;
	std::string line,nonterminal;
	std::ifstream in(filename.c_str());
	while(getline(in,line)){
		std::string::size_type beginPos = line.find_first_not_of(" \t");
		if(beginPos!= std::string::npos){
			std::string::size_type endPos = line.find("->",beginPos);
			if( endPos != std::string::npos){
				nonterminal = line.substr(beginPos,endPos-beginPos);
				// To remove spaces from nonterminal if any.
				beginPos = nonterminal.find_first_of(" \t",0);
				if(beginPos != std::string::npos){
					nonterminal = nonterminal.substr(0,beginPos);
				}
				// To store the starting symbol of the language.
				if(setNonTerminals.empty()){
					startSymbol = nonterminal;
				}
				setNonTerminals.insert(nonterminal);
				line = line.substr(endPos+2);
				_productions = tokenize(line," |\t");
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
	
	if(removeLeftRecursion()){
		std::cout<<"Grammer contained left recursion, check the new production table to see the modified grammer without left recursion."<<std::endl;
	}
	if(leftFactor()){
		std::cout<<"Grammer was not left factored, check the new production table to see the modified left factored grammer."<<std::endl;
	}
	optimizeTable();
}

LLGrammar::~LLGrammar(){

}

bool LLGrammar::removeLeftRecursion(){
	bool leftRecursion, containsLeftRecursion = false;
	std::string::size_type pos;
	std::string startTokenSymbol,restOfProduction, nextNonTerminalSymbol;
	//To remove indirect left recursion
	for(int i=0;i<nonTerminals.size();i++){
		nextNonTerminalSymbol = getNextNonTerminalSymbol(nonTerminals[i]);
		for(int k=0;k<productions[nonTerminals[i]].size();k++){
			pos = productions[nonTerminals[i]][k].find(".");
			if(pos == std::string::npos){
				startTokenSymbol = productions[nonTerminals[i]][k];
				restOfProduction = "@";
			}else{
				startTokenSymbol = productions[nonTerminals[i]][k].substr(0,pos);
				restOfProduction = productions[nonTerminals[i]][k].substr(pos+1);
			}
			for(int j=0; j<i; j++){				
				if(startTokenSymbol == nonTerminals[j]){
					for(int m=0;m<productions[nonTerminals[j]].size();m++){
						productions[nonTerminals[i]].push_back(productions[nonTerminals[j]][m]+"."+restOfProduction);
					}
					productions[nonTerminals[i]].erase(productions[nonTerminals[i]].begin()+k);
					k--;
					break;
				}
			}
		}
		//To remove immediate left recursion
		bool leftRecursion=false;
		for(int k=0; k < productions[nonTerminals[i]].size(); k++){
			pos = productions[nonTerminals[i]][k].find(".");
			if(pos == std::string::npos){
				startTokenSymbol = productions[nonTerminals[i]][k];
				restOfProduction = "@";
			}else{
				startTokenSymbol = productions[nonTerminals[i]][k].substr(0,pos);
				restOfProduction = productions[nonTerminals[i]][k].substr(pos+1);
			}
			//std::cout<<nonTerminals[i]<< productions[nonTerminals[i]].size()<<" : "<<startTokenSymbol<<" "<<restOfProduction<<std::endl;
			if(startTokenSymbol == nonTerminals[i]){
				containsLeftRecursion = true;
				leftRecursion = true;
				productions[nextNonTerminalSymbol].push_back(restOfProduction+"."+nextNonTerminalSymbol);
				productions[nonTerminals[i]].erase(productions[nonTerminals[i]].begin()+k);
				k--;
			}
		}
		if(leftRecursion){
			productions[nextNonTerminalSymbol].push_back("@");
			nonTerminals.push_back(nextNonTerminalSymbol);
			for(int k=0; k < productions[nonTerminals[i]].size(); k++){
				productions[nonTerminals[i]][k] = productions[nonTerminals[i]][k]+"."+nextNonTerminalSymbol;
			}
		}
	}
	return containsLeftRecursion;
}

bool LLGrammar::leftFactor(){
	bool wasNotLeftFactored = false;
	std::string::size_type pos;
	std::string startTokenSymbol,restOfProduction,nextNonTerminalSymbol;
	std::map<std::string, std::vector< std::pair<std::string, int> > > countMap;
	for(int i=0;i<nonTerminals.size();i++){
		countMap.clear();
		for(int k=0;k<productions[nonTerminals[i]].size();k++){
			pos = productions[nonTerminals[i]][k].find(".");
			if(pos == std::string::npos){
				startTokenSymbol = productions[nonTerminals[i]][k];
				restOfProduction = "@";
			}else{
				startTokenSymbol = productions[nonTerminals[i]][k].substr(0,pos);
				restOfProduction = productions[nonTerminals[i]][k].substr(pos+1);
			}
			countMap[startTokenSymbol].push_back(make_pair(restOfProduction,k));
		}
		for(std::map<std::string, std::vector< std::pair<std::string, int> > >::iterator it=countMap.begin(); it!= countMap.end();it++){
			if((*it).second.size() > 1){
				wasNotLeftFactored = true;
				nextNonTerminalSymbol = getNextNonTerminalSymbol(nonTerminals[i]);
				nonTerminals.push_back(nextNonTerminalSymbol);
				for(int j=0;j<(*it).second.size(); j++){
					productions[nextNonTerminalSymbol].push_back((*it).second[j].first);
					productions[nonTerminals[i]][(*it).second[j].second] = (*it).first +"."+nextNonTerminalSymbol;
				}
			}
		}
	}
	return wasNotLeftFactored;
}

void LLGrammar::optimizeTable(){
	bool wasNotLeftFactored = false;
	std::string::size_type pos;
	std::string startTokenSymbol,restOfProduction,nextNonTerminalSymbol;
	std::map<std::string, std::vector< std::pair<std::string, int> > > countMap;
	for(int i=0;i<nonTerminals.size();i++){
		for(int j=0; j < productions[nonTerminals[i]].size(); j++ ){
			productions[nonTerminals[i]][j] = removeExtraEplisons(productions[nonTerminals[i]][j]);
		}
		productions[nonTerminals[i]] = removeDuplicates(productions[nonTerminals[i]]);
		if(productions[nonTerminals[i]].size()==0){
			nonTerminals.erase(nonTerminals.begin()+i);
			productions.erase(nonTerminals[i]);
			i--;
		}
	}
}

std::vector<std::string> LLGrammar::removeDuplicates(std::vector<std::string> vec){
	std::vector<std::string> retVec;
	std::map<std::string, int > countMap;
	for(int i=0;i<vec.size();i++){
		if(countMap.find(vec[i])!=countMap.end()){
			countMap[vec[i]]++;
		}
		else{
			countMap[vec[i]] = 1;
			retVec.push_back(vec[i]);
		}
	}
	return retVec;
}

std::string LLGrammar::removeExtraEplisons(std::string str){
	std::vector<std::string> _tokens = tokenize(str,".");
	std::map<std::string, std::vector<int> > posMap;
	std::string retStr="";
	posMap["@"].clear();
	for(int i=0;i<_tokens.size();i++){
		posMap[_tokens[i]].push_back(i);
	}
	if(posMap["@"].size() == _tokens.size()){
		retStr = "@";
	}else{
		_tokens.erase(std::remove(_tokens.begin(), _tokens.end(), "@"), _tokens.end());
		for(int i=0;i<_tokens.size()-1;i++){
			retStr += _tokens[i]+".";
		}
		retStr += _tokens[_tokens.size()-1];
	}
	return retStr;
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

	std::set<std::string> firstSet, firstProductionSet, tempSet;
	std::vector<std::string> _symbols;

	for(int i=0 ; i<productions[symbol].size(); i++){
		firstProductionSet.clear();
		_symbols = tokenize(productions[symbol][i],".");
		for(int j = 0; j<_symbols.size(); j++){
			tempSet=computeFirst(_symbols[j]);
			std::copy(tempSet.begin(), tempSet.end(), std::inserter(firstProductionSet, firstProductionSet.end()));
			if(!containsEpsilon(_symbols[j])){
				break;
			}
		}
		for(std::set<std::string>::iterator it = firstProductionSet.begin(); it != firstProductionSet.end(); it++){
			if(firstSet.find(*it)!=firstSet.end()){				
				std::cout<<"Terminal : "<<*it<<" is present in first sets of muliple productions for "<<symbol<<std::endl;
				isLL1 = false;
				break;
			}else{
				firstSet.insert(*it);
			}
		}
	}

	return firstSet;
}

void LLGrammar::computeFollow(std::string symbol,std::map<std::string, std::vector<std::string> >& tempFollowSets ){
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

void LLGrammar::computeFirstSets(bool print){

	if(!firstSetsComputed){
		firstSetsComputed = true;
		computeEpsilonSets();

		firstSets["@"].insert("@");
		for(int i = 0; i<terminals.size(); i++){
			firstSets[terminals[i]].clear();
			firstSets[terminals[i]].insert(terminals[i]);
		}

		for(int i = 0; i<nonTerminals.size(); i++){
			if(firstSets.find(nonTerminals[i]) == firstSets.end()){
				firstSets[nonTerminals[i]]=computeFirst(nonTerminals[i]);
				if(!isLL1){
					std::cout<<"First sets of productions for "<<nonTerminals[i]<<" are not dis-joint."<<std::endl;
					//return;
				}
			}
		}
	}
	if(print && isLL1){
		std::cout<<"First Set is  : "<<std::endl;
		std::map<std::string, std::set<std::string> >::iterator it;
		std::set<std::string>::iterator itSet;
		for(int i = 0; i<terminals.size(); i++){
			std::cout<<terminals[i]<<"\t->\t";
			for(itSet=firstSets[terminals[i]].begin();itSet != firstSets[terminals[i]].end();itSet++){
				std::cout<<(*itSet)<<"\t|\t";
			}
			std::cout<<std::endl;
		}

		for(int i = 0; i<nonTerminals.size(); i++){
			std::cout<<nonTerminals[i]<<"\t->\t";
			for(itSet=firstSets[nonTerminals[i]].begin();itSet != firstSets[nonTerminals[i]].end();itSet++){
				std::cout<<(*itSet)<<"\t|\t";
			}
			std::cout<<std::endl;
		}
	}
}

void LLGrammar::computeFollowSets(bool print){
	if(!isLL1){
		std::cout<<"Grammar is not LL(1). Please make the grammar LL(1) before computing follow sets."<<std::endl;
		return;
	}

	if(firstSetsComputed==false){
		std::cout<<"Warning : First sets not computed. Computing first sets."<<std::endl;
		computeFirstSets(false);
		if(!isLL1){
			return;
		}
	}
	if(!followSetsComputed){
		followSetsComputed = true;
		std::map<std::string, std::vector<std::string> > tempFollowSets;
		std::map<std::string, std::vector<std::string> >::iterator it;
		std::vector<std::string> tempVector, bfsVector;;
		std::string symbol, _symbol; 
		for(int i=0;i<nonTerminals.size();i++){
			computeFollow(nonTerminals[i],tempFollowSets);
		}

		for(it=tempFollowSets.begin();it!=tempFollowSets.end();it++){
			tempVector = (*it).second;
			removeDuplicates(tempVector);
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
	}

	if(print){
		std::cout<<"Follow Set is  : "<<std::endl;
	}
	std::map<std::string, std::set<std::string> >::iterator itFollowSets;
	for(itFollowSets=followSets.begin();itFollowSets!=followSets.end();itFollowSets++){
		if((*itFollowSets).second.find("@")!=(*itFollowSets).second.end()){	
			(*itFollowSets).second.erase((*itFollowSets).second.find("@"));
		}
		if(print){
			std::cout<<(*itFollowSets).first<<"\t->\t";
			for(std::set<std::string>::iterator itSet=(*itFollowSets).second.begin();itSet != (*itFollowSets).second.end();itSet++){
				std::cout<<(*itSet)<<"\t|\t";
			}
			std::cout<<std::endl;
		}
	}
	std::set<std::string> firstSet,followSet;
	for(int i=0; i < nonTerminals.size(); i++){
		if(containsEpsilon(nonTerminals[i])){
			followSet = getFollow(nonTerminals[i]);
			firstSet = getFirst(nonTerminals[i]);
			for(std::set<std::string>::iterator it = firstSet.begin(); it != firstSet.end(); it++){
				if(followSet.find(*it)!=followSet.end()){				
					std::cout<<"Terminal : "<<*it<<" is present in first sets and follow set of "<<nonTerminals[i]<<std::endl;
					std::cout<<"First and Follow sets are not disjoint for "<<nonTerminals[i]<<"."<<std::endl;
					isLL1 = false;
					return;
				}
			}
		}
	}
}

void LLGrammar::parseTableConstruction(){
	if(!isLL1){
		std::cout<<"Grammar is not LL(1). Please make the grammar LL(1) before creating parse table."<<std::endl;
		return;
	}
	if(followSetsComputed==false){
		std::cout<<"Warning : Follow sets not computed. Computing follow sets."<<std::endl;
		computeFollowSets(false);
		if(!isLL1){
			return;
		}
	}
	int maxLengthProductions = 0, maxLengthSymbol = 0;
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
			// To get the maximum string length of a production for better formatting of table in output.
			maxLengthProductions = std::max(maxLengthProductions,(int)productions[nonTerminals[i]][j].length());
		}
		maxLengthSymbol = std::max(maxLengthSymbol,(int)nonTerminals[i].length());
	}
	out << startSymbol<<std::endl ;
	std::stringstream printLine ;
	printLine << "|"<<std::setw(maxLengthSymbol+1) ;
	for(int j=0 ; j<terminals.size() ;j++){
		printLine<<"|"<<std::setw(maxLengthProductions)<< terminals[j];
	}
	printLine <<"|"<<std::setw(maxLengthProductions)<<"$"<<std::endl ;
	out<<printLine.str();
	out<<std::setw(printLine.str().length())<<std::setfill('-')<<'-'<<std::endl<<std::setfill(' ');
	for(int i=0 ; i<nonTerminals.size() ; i++){
		out<<"|"<<std::setw(maxLengthSymbol)<< nonTerminals[i];
		for(int j=0 ; j<terminals.size() ;j++){
			tempPair = std::make_pair(nonTerminals[i],terminals[j]);
			out <<"|"<<std::setw(maxLengthProductions)<<parseTable[tempPair];
		}
		tempPair = std::make_pair(nonTerminals[i],"$");
		out<<"|"<<std::setw(maxLengthProductions)<<parseTable[tempPair] << std::endl ;
	}
	out.close() ;
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
			std::cout<<(*it).first<<"\t->\t";
			for(int i=0;i<(*it).second.size();i++){
				std::cout<<(*it).second[i]<<"\t|\t";
			}
			std::cout<<std::endl;
		}
	}
	return productions;
}

std::string LLGrammar::getNextNonTerminalSymbol(std::string symbol){
	if(find(nonTerminals.begin(),nonTerminals.end(),symbol) != nonTerminals.end()){
		return getNextNonTerminalSymbol(symbol+"'");
	}
	else{
		return symbol;
	}
}

// This function tokenizes the string on the basis of delimeters space or newline or cariage return.
std::vector<std::string> LLGrammar::tokenize(std::string s, std::string sep){
	// Skip delimiters at beginning.
	std::string::size_type lastPos = s.find_first_not_of(sep, 0);
	// Find first "non-delimiter", which will be between lastPos and pos
	std::string::size_type pos = s.find_first_of(sep, lastPos); 
	std::vector<std::string> _tokens;
	while(pos != std::string::npos || lastPos != std::string::npos){
		_tokens.push_back(s.substr(lastPos,(pos - lastPos)));
		// Skip delimiters
		lastPos = s.find_first_not_of(sep, pos);
		// Find "non-delimiter", which will be between lastPos and pos
		pos = s.find_first_of(sep, lastPos); 
	}
	return _tokens;
}