#include "headers/parser.h"

int main(){

	std::vector<std::string> input;

	input.push_back("int");
	input.push_back("*");
	input.push_back("int");
	input.push_back("$");

	LLParser parser(input);
	std::cout<<parser.parse(true);

	return 0;
}