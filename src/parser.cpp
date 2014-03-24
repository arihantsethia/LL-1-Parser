#include "headers/parser.h"

LLParser::LLParser(){

}

LLParser::~LLParser(){

}

LLParser::LLParser(std::vector<std::string> _tokens){
	std::string line,_nonTerminal,_terminal;
	std::pair<std::string,std::string> tempPair;
	std::vector<std::string> _symbols,tableSymbols;
	std::ifstream in("parse_table.txt");

	tokens = _tokens;
	getline(in,startSymbol);
	getline(in,line);
	tableSymbols = tokenize(line," \t|");
	terminals = tableSymbols;
	terminals.erase(terminals.end()-1);
	getline(in,line);
	while(getline(in,line)){
		_symbols=tokenize(line," \t|");
		_nonTerminal = _symbols[0];
		nonTerminals.push_back(_nonTerminal);
		for(int i=0;i<tableSymbols.size();i++){
			_terminal = tableSymbols[i];
			tempPair=std::make_pair(_nonTerminal,_terminal);
			parseTable[tempPair] = _symbols[i+1];
		}
	}
}

bool LLParser::parse(bool print){
	int pos = 0;
	std::vector<std::string> _symbols;
	std::stack<std::string> parseStack;
	std::pair<std::string,std::string> tempPair;

	parseStack.push("$");
	parseStack.push(startSymbol);

	while(pos<tokens.size()){
		if(find(terminals.begin(),terminals.end(),parseStack.top()) != terminals.end()){
			if(parseStack.top() == tokens[pos]){
				if(print){
					std::cout<<"POP : "<<parseStack.top()<<std::endl;
				}
				parseStack.pop();
				pos++;
			}
			else{
				std::cout<<parseStack.top()<<" : "<<tokens[pos]<<std::endl;
				return false;
			}
		}
		else if(parseStack.top() == "$"){
			if(tokens[pos] == "$"){
				return true;
			}
			else{
				return false;
			}
		}
		else if(find(nonTerminals.begin(),nonTerminals.end(),parseStack.top()) != nonTerminals.end()){
			tempPair=std::make_pair(parseStack.top(),tokens[pos]);
			if(parseTable[tempPair] != "error"){
				if(print){
					std::cout<<"POP : "<<parseStack.top()<<std::endl;
				}
				parseStack.pop();
				_symbols = tokenize(parseTable[tempPair],".");
				if(print){
					std::cout<<"PUSH : ";
				}
				for(int j=_symbols.size()-1;j>=0;j--){
					if(_symbols[j]!="@"){
						if(print){
							std::cout<<_symbols[j]<<".";
						}
						parseStack.push(_symbols[j]);
					}
				}
				if(print){
					std::cout<<std::endl;
				}
			}
			else{
				return false;
			}
		}
	}	
	return true;
}

// This function tokenizes the string on the basis of delimeters space or newline or cariage return.
std::vector<std::string> LLParser::tokenize(std::string s, std::string sep){
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