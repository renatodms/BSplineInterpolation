#include "main.h"

int qnt_pontos;
Ponto pnts[1000];
GLfloat mouse_x, mouse_y;

//Estado inicial
void init(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	srand(time(NULL));
	qnt_pontos = 0;
}

//Printa um ponto nas coordenadas (x,y)
void draw(GLint x, GLint y){
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 8; i++) {
			float angulo = i * 2 * M_PI / 8;
			glVertex2f(x + (cos(angulo) * 4), y + (sin(angulo) * 4));
		}
	glEnd();
}

//Rotina magica (copiada do Template2D.cpp)
void reshape(GLsizei w, GLsizei h){
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat)w, (GLfloat)h, 0.0f, -5.0, 5.0);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT);

	//Pinta todos os pontos de 'pnts'
	for (int i=0; i<qnt_pontos;++i){
		draw(pnts[i].x, pnts[i].y);
	}

	glFlush();
}

void handleMouse(int btn, int state, int x, int y){
	//Printa as coordenadas clicando com o botao direito do mouse
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		cout << "(" << x << ", " << y << ")" << endl;
	}
	//Adiciona o ponto clicando com o botao esquerdo do mouse
	if(btn == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN){
			pnts[qnt_pontos++] = Ponto(x, y);
		}
	}
}

//Fecha aplicacao ao apertar ESC
void hadleKeyboard(unsigned char key, int x, int y){
	if(key == ESC){
		exit(0);
	}
}

//Reinicia ao apertar F5
void hadleSpecialKeyboard(int key, int x, int y){
	if(key == GLUT_KEY_F5){
		init();
	}
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(960, 550);
	glutCreateWindow("BSplineInterpolation");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(handleMouse);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	init();

	glutMainLoop();
	return 0;
}