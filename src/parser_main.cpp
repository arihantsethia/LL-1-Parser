#include "headers/parser.h"

int main(){

	std::vector<std::string> input;

	input.push_back("int");
	input.push_back("*");
	input.push_back("int");
	input.push_back("$");

	LLParser parser(input);
	if(parser.parse(true)){
		std::cout<<"Succesfull. :)"<<std::endl;
	}else{
		std::cout<<"Error : Not a valid instance of the language."<<std::endl;
	}

	return 0;
}