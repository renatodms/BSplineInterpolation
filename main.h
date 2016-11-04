#define _USE_MATH_DEFINES //Para usar as constantes de math.h
#define ESC 27 //Codigo da tecla ESC
#define SLD_MAX 960 //Valor maximo do slide

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <math.h>
#include <gl/glut.h>
#include <iostream>
using namespace std;

struct Ponto{
	GLfloat x, y;
	Ponto(GLfloat x=0.0, GLfloat y=0.0):x(x),y(y){}
};

struct Slide{
	GLfloat x;
	Slide(GLfloat x=0.0):x(x){}
};