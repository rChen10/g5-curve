#include <iostream>
#include <fstream>
#include <cmath>

#include "matrix.h"

#ifndef __POLYGON_H__
#define __POLYGON_H_


class polygon : public matrix{

};


#define PI atan(1)*4
//helper
static double convert(double theta){
	return theta * PI / 180;
}


static matrix create_translate(std::vector<double> p){
	matrix m = matrix(4, 4);
	m.ident();
	for (int i = 0; i < 3; i++){
		m[m.size()-1][i] = p[i];
	}
	return m;
}

static matrix create_scale(std::vector<double> a){
	matrix m = matrix(4, 4);
	m.ident();
	for (int i = 0; i < 3; i++){
		(m[i])[i] = a[i];
	}
	return m;
}

static matrix create_rotateZ(double theta){
	std::cout<<"theta:"<<theta<<"\n";
	double angle = convert(theta);
	std::cout<<angle<<"\n";
	matrix m = matrix(4, 4);
	m.ident();
	m[0][0] = cos(angle);
	m[1][0] = -sin(angle);
	m[0][1] = sin(angle);
	m[1][1] = cos(angle);
	return m;

}

static matrix create_rotateX(double theta){
	
	double angle = convert(theta);
	matrix m = matrix(4, 4);
	m.ident();
	m[1][1] = cos(angle);
	m[2][1] = -sin(angle);
	m[1][2] = sin(angle);
	m[2][2] = cos(angle);
	return m;

}

static matrix create_rotateY(double theta){

	double angle = convert(theta);
	matrix m = matrix(4, 4);
	m.ident();
	m[2][2] = cos(angle);
	m[0][2] = -sin(angle);
	m[2][0] = sin(angle);
	m[0][0] = cos(angle);
	return m;

}

static matrix make_curve(matrix& coefs, int step){
	matrix m = matrix();
	double add = 1 / ((double) step);
	std::vector<double> *pi;
	std::vector<double> *pi2;
	for(double t = 0; t <= 1; t+=add){
		pi = new std::vector<double>(4, 0);
		(*pi)[0] = (coefs[0][0] * pow(t,3)) + (coefs[0][1] * pow(t,2)) + (coefs[0][2] * t) + coefs[0][3];
		(*pi)[1] = (coefs[1][0] * pow(t,3)) + (coefs[1][1] * pow(t,2)) + (coefs[1][2] * t) + coefs[1][3];
		(*pi)[2] = 0;
		(*pi)[3] = 1;
		m += (*pi);

		pi2 = new std::vector<double>(4, 0);
		(*pi2)[0] = (coefs[0][0] * pow(t+add, 3)) + (coefs[0][1] * pow(t+add, 2)) + (coefs[0][2] * (t+add)) + coefs[0][3];
		(*pi2)[1] = (coefs[1][0] * pow(t+add, 3)) + (coefs[1][1] * pow(t+add, 2)) + (coefs[1][2] * (t+add)) + coefs[1][3];
		(*pi2)[2] = 0;
		(*pi2)[3] = 1;
		m += (*pi2);
		std::cout<<"lol: "<<coefs[0][0]<<" "<<coefs[0][1]<<" "<<coefs[0][2]<<" "<<coefs[0][3]<<"\n";
		//std::cout<<(*pi)[0]<<" "<<(*pi)[1]<<" "<<(*pi)[2]<<" "<<(*pi)[3]<<"\n";
	}
	return m;
}

static matrix make_circle(double r, std::vector<double>& pc, int step){
	matrix m = matrix();
	double add = 1 / ((double) step);
	std::vector<double> *pi;
	std::vector<double> *pi2;
	for(double t = 0; t <= 1; t+=add){
		pi = new std::vector<double>(4, 0);
		(*pi)[0] = r * cos( 2 * PI * t) + pc[0];
		(*pi)[1] = r * sin( 2 * PI * t) + pc[1];
		(*pi)[2] = pc[2];
		(*pi)[3] = 1;
		m += (*pi);

		pi2 = new std::vector<double>(4, 0);
		(*pi2)[0] = r * cos( 2 * PI * (t+add)) + pc[0];
		(*pi2)[1] = r * sin( 2 * PI * (t+add)) + pc[1];
		(*pi2)[2] = pc[2];
		(*pi2)[3] = 1;
		m += (*pi2);
		//std::cout<<(*pi)[0]<<" "<<(*pi)[1]<<" "<<(*pi)[2]<<" "<<(*pi)[3]<<"\n";
	}
	return m;
}

static matrix generate_curve_coefs(matrix& a, int type){
	matrix coefs;
	if(type == 0){
		matrix m = matrix();
		point r1 = point(2, -3, 0, 1);
		point r2 = point(-2, 3, 0, 0);
		point r3 = point(1, -2, 1, 0);
		point r4 = point(1, -1, 0, 0);
		m+=r1;m+=r2;m+=r3;m+=r4;

		coefs = m * a;
	}

	else{	
		matrix m = matrix();
		point r1 = point(-1, 3, -3, 1);
		point r2 = point(3, -6, 3, 0);
		point r3 = point(-3, 3, 0, 0);
		point r4 = point(1, 0, 0, 0);
		m+=r1;m+=r2;m+=r3;m+=r4;
		
		//std::cout<<a;
		coefs = m * a;
	}
	return coefs;
}


#endif
