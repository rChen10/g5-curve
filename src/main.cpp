#include "../include/parser.h"

int main(int argc, char **argv){


	screen s = screen(500, 500);
	matrix transform = matrix(4, 4);
	matrix edges = matrix();
	if(argc > 0){
		std::cout << "reading script...";
		parse_file(argv[1], transform, edges, s);
	}
	else
		return 1;
}
