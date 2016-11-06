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

void init();
void algFMILL();
void algBessel();
void pntsAtt();
void gerarPontos(GLint x, GLint y);
float fat(int x);
float comb(int a, int b);
void desenhaPonto(GLint x, GLint y);
void ligaPontos(GLint x1, GLint y1, GLint x2, GLint y2);
void bezier(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4, GLfloat n);
void reshape(GLsizei w, GLsizei h);
void display();
void handleMouse(int btn, int state, int x, int y);
void handleMotion(int x, int y);
void hadleKeyboard(unsigned char key, int x, int y);
void hadleSpecialKeyboard(int key, int x, int y);