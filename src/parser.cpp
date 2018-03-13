#include "../include/parser.h"
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

char ** parse_args( char *line ){
  char **args = (char **)calloc(10, sizeof(char *));
  char *temp = 0;
  int i = 0;
  while (temp = strsep(&line, " ")){
    args[i] = temp;
    i++;
  }
  return args;
}

void parse_line(char *str, matrix& edges){
	char **args = parse_args(str);
	std::cout<<"\n";
	point *p;
	for(int i = 0; i < 2; i++){
		p = new point(atoi(args[(i*3) + 0]), atoi(args[(i*3) + 1]), atoi(args[(i*3) + 2]));
		edges+=(*p);
	}

}

void parse_scale(char *str, matrix& transform){
	char **args = parse_args(str);
	std::vector<double> p;
	for (int j = 0; j < 3; j++){
		p.push_back(atof(args[j]));
	}
	matrix scale = create_scale(p);
	transform = scale * transform;
}

void parse_translate(char *str, matrix& transform){
	char **args = parse_args(str);
	std::vector<double> p;
	for (int j = 0; j < 3; j++){
		p.push_back(atof(args[j]));
	}
	matrix translate = create_translate(p);
	transform = translate * transform;
}

void parse_rotate(char *str, matrix& transform){
	char **args = parse_args(str);
	matrix rotate = matrix(4, 4);
	rotate.ident();
	if (strncmp(args[0], "x", 1) == 0){
		std::cout<<"rotating\n";
		rotate = create_rotateX(atof(args[1]));
	}
	else if (strncmp(args[0], "y", 1) == 0){
		std::cout<<"rotating\n";
		rotate = create_rotateY(atof(args[1]));
	}
	else if (strncmp(args[0], "z", 1) == 0){
		std::cout<<"rotating\n";
		rotate = create_rotateZ(atof(args[1]));
	}
	transform = rotate * transform;

}

void parse_display(screen& s, matrix& m){
	FILE *f;
	m.draw(s);
	f = popen("display", "w");
	s.render(f);
	pclose(f);
	std::cout<<"rendered\n";
}

void parse_hermite(char *str, matrix& edges){	
	std::vector<double> x = std::vector<double>(4, 0); std::vector<double> y = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	int j = 0;
	for(int i = 0; i < 8; i+=2){
		x[j] = atof(args[i]);
		y[j] = atof(args[i+1]);
		j++;
	}
	matrix intermediate = matrix(); intermediate+=x; intermediate+=y;
	matrix coeffs = generate_curve_coefs(intermediate, 0);
	matrix hermite = make_curve(coeffs, step);
	edges+=hermite;
	std::cout<<"herm\n";
}
void parse_bezier(char *str, matrix& edges){
	std::cout<<str<<"\n";
	std::vector<double> x = std::vector<double>(4, 0); std::vector<double> y = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	int j = 0;
	for(int i = 0; i < 8; i+=2){
		x[j] = atof(args[i]);
		y[j] = atof(args[i+1]);
		j++;
	}
	matrix intermediate = matrix(); intermediate+=x; intermediate+=y;
	matrix coeffs = generate_curve_coefs(intermediate, 1);
	matrix bezier = make_curve(coeffs, step);
	edges+=bezier;
	std::cout<<"bez\n";
}
void parse_circle(char *str, matrix& edges){
	double r; std::vector<double> pc = std::vector<double>(4, 0); int step = 25;
	char **args = parse_args(str);
	for(int i = 0; i < 3; i++){
		pc[i] = atof(args[i]);
	}
	r = atof(args[3]);
	matrix circle = make_circle(r, pc, step);
	edges += circle;
}
void parse_save(screen& s, matrix& m, char *str){
	m.draw(s);
	s.render(str);
}
void parse_file ( const char * filename, 
		  matrix&  transform, 
		  matrix&  edges,
		  screen& s){
	std::ifstream f;
	f.open(filename);
	std::cout<<"file read\n";
	char * str = (char *) calloc(250, sizeof(char));
	
	f.getline(str, 250);


	while ( (f.rdstate() & std::ifstream::eofbit ) == 0 ){
		if(strncmp(str, "line", 250) == 0){
			f.getline(str, 250);
			parse_line(str, edges);
		}
		
		else if(strncmp(str, "ident", 250) == 0){
			transform.ident();
		}

		else if(strncmp(str, "scale", 250) == 0){
			f.getline(str, 250);
			parse_scale(str, transform);
		}

		else if(strncmp(str, "move", 250) == 0){
			f.getline(str, 250);
			parse_translate(str, transform);
		}

		else if(strncmp(str, "rotate", 250) == 0){
			f.getline(str, 250);
			parse_rotate(str, transform);
		}

		else if(strncmp(str, "apply", 250) == 0){
			edges = transform * edges;
		}

		else if(strncmp(str, "display", 250) == 0){
			s.clear();
			parse_display(s, edges);
		}

		else if(strncmp(str, "save", 250) == 0){
			s.clear();
			f.getline(str, 250);
			parse_save(s, edges, str);
		}

		else if(strncmp(str, "hermite", 250) == 0){
			f.getline(str, 250);
			parse_hermite(str, edges);
		}
		
		else if(strncmp(str, "bezier", 250) == 0){
			f.getline(str, 250);
			parse_bezier(str, edges);
		}

		else if(strncmp(str, "circle", 250) == 0){
			f.getline(str, 250);
			parse_circle(str, edges);
		}

		f.getline(str, 250);
	}
}
