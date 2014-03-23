#include "headers/parser.h"

int main(){
	std::string line,filename;
	std::vector<std::string> input;
	std::cout<<"Enter the file name containing a list of tokens. : ";
	std::cin>>filename;
	std::ifstream in(filename.c_str());
	if(in){
		while(getline(in,line)){
			input.push_back(line);
		}		
		input.push_back("$");
		LLParser parser(input);
		if(parser.parse(true)){
			std::cout<<"Succesfull. :)"<<std::endl;
		}else{
			std::cout<<"Error : Not a valid instance of the language."<<std::endl;
		}
	}else{
		std::cout<<"File doesn't exist."<<std::endl;
	}
	return 0;
}