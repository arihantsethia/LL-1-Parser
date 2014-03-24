#include <iostream>
#include <cstdio>
#include <cstdlib>

extern int  yyparse();
extern FILE *yyin;

int main() {
	std::string filename;
	std::cout<<"Enter the filename : ";
	getline(std::cin,filename);
	FILE *fp=fopen(filename.c_str(),"r");
 	if(!fp){
		printf("couldn't open file for reading\n");
		return 0;
	}
	yyin=fp;
	yyparse();
	fclose(fp);
	return 0;
}