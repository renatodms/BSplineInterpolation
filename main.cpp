/*************************************************
	F5 - Reinicia
	F4 - Mostrar/ocultar poligonais de controle
	F3 - Curva fechada/aberta
	F2 - Escolher calculo da tangente	
**************************************************/
#include "main.h"

int qnt_pontos;
Ponto pnts[100];
int qnt_slides;
Slide slds[30];
GLfloat mouse_x, mouse_y;
bool showPoli;
bool fechada;
bool tangente;
int movendo;
int sliding;

//Estado inicial
void init(){
	glClearColor(1.0, 1.0, 1.0, 1.0);
	srand(time(NULL));
	qnt_pontos = 0;
	qnt_slides = 0;
	showPoli = false;
	fechada = false;
	tangente = true;
	movendo = -1;
	sliding = -1;
}

//Calcula tangente usando FMILL
void algFMILL(){
	for(int i=3; i<qnt_pontos; ++i){
		if(i%3 == 0 && i+3<qnt_pontos){
			Ponto xB = pnts[i-3], x = pnts[i], xA = pnts[i+3];
			GLfloat v[2] = {xA.x-xB.x, xA.y-xB.y};
			pnts[i-1] = Ponto(x.x-(pnts[i].u)*v[0], x.y-(pnts[i].u)*v[1]);
			pnts[i+1] = Ponto(x.x+(pnts[i].u)*v[0], x.y+(pnts[i].u)*v[1]);
		}
	}
	if(fechada){
		Ponto xB = pnts[qnt_pontos-4], x = pnts[qnt_pontos-1], xA = pnts[0];
		GLfloat v[2] = {xA.x-xB.x, xA.y-xB.y};
		pnts[qnt_pontos-2] = Ponto(x.x-(pnts[qnt_pontos-2].u)*v[0], x.y-(pnts[qnt_pontos-2].u)*v[1]);
		pnts[1] = Ponto(x.x+(pnts[1].u)*v[0], x.y+(pnts[1].u)*v[1]);
	}
}

//Calcula tangente usando Bessel
void algBessel(){
	if(!fechada){
		gerarPontos(pnts[0].x, pnts[0].y);
		pnts[qnt_pontos++] = pnts[0];
		algFMILL();
		Ponto xB = pnts[qnt_pontos-4], x = pnts[qnt_pontos-1], xA = pnts[0];
		GLfloat v[2] = {xA.x-xB.x, xA.y-xB.y};
		pnts[1] = Ponto(x.x+(0.1)*v[0], x.y+(0.1)*v[1]);
		qnt_pontos-=3;
	} else algFMILL();
}

//Recalcular pnts
void pntsAtt(){
	if (tangente){
		algFMILL();
	} else {
		algBessel();
	}
}

//Gera dois pontos iguais entre o ponto (x,y) e o anterior
void gerarPontos(GLint x, GLint y){
	pnts[qnt_pontos++] = Ponto((pnts[qnt_pontos-1].x+x)/2, (pnts[qnt_pontos-1].y+y)/2);
	pnts[qnt_pontos++] = Ponto(pnts[qnt_pontos-1].x, pnts[qnt_pontos-1].y);
}

//Calcula o fatorial de x
float fat(int x){
	float n = 1.0f;
	for (int i = x; i > 0; --i){
		n *= (float)i;
	}
	return n;
}

//Calcula combinacao (a b)
float comb(int a, int b){
	return fat(b)/(fat(a)*fat(b-a));
}

//Printa um ponto nas coordenadas (x,y)
void desenhaPonto(GLint x, GLint y){
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 8; i++) {
			float angulo = i * 2 * M_PI / 8;
			glVertex2f(x + (cos(angulo) * 4), y + (sin(angulo) * 4));
		}
	glEnd();
}

//Liga os pontos (x1, y1) e (x2, y2)
void ligaPontos(GLint x1, GLint y1, GLint x2, GLint y2){
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);
		glVertex2i(x1, y1);
		glVertex2i(x2, y2);
	glEnd();
}

//Desenha curva de bezier de grau 3 entres os pontos (x1, y1), (x2, y2), (x3, y3) e (x4, y4) com suavidade n
void bezier(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3, GLint x4, GLint y4, GLfloat n){
	glColor3f(0.0f,1.0f,0.0f);
	glBegin(GL_LINES);
		for(GLfloat u = 0.0f; u <= 1.0f; u+=n){
			GLfloat x = 0.0f;
			GLfloat y = 0.0f;
			
			//Qualcular coordenadas para cada ponto da curva de bezier (Algoritmo de De Casteljau)
			int i = 0;
			x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x1;
			y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y1;
			i = 1;
			x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x2;
			y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y2;
			i = 2;
			x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x3;
			y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y3;
			i = 3;
			x += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*x4;
			y += comb(i,3)*pow(u,i)*pow(1.0f-u,3-i)*y4;
			
			glVertex2f(x,y);
			if(u>0.0) glVertex2f(x,y);
		}
		glVertex2f(x4,y4);
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
	pntsAtt();

	//Pinta todos os pontos de 'pnts'
	if(fechada && qnt_pontos != 0){
		gerarPontos(pnts[0].x, pnts[0].y);
		pnts[qnt_pontos++] = pnts[0];
		pntsAtt();
	}
	for (int i=0; i<qnt_pontos; ++i){
		if (i == 0 || i%3 == 0 || showPoli) desenhaPonto(pnts[i].x, pnts[i].y);
		if (i>0 && showPoli) ligaPontos(pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y);
		float u;
		if ((i/3)-1 == 0 && qnt_pontos>4) u = 9.6/((960*(slds[(i/3)-1].x))/(slds[(i/3)].x));
		else if ((i/3)-1 == qnt_slides-1) u = 9.6/((960*(slds[(i/3)-1].x-slds[(i/3)-2].x))/(960-slds[(i/3)-2].x));
		else if (qnt_pontos>4) u = 9.6/((960*(slds[(i/3)-1].x-slds[(i/3)-2].x))/(slds[(i/3)].x-slds[(i/3)-2].x));
		else u = 9.6/slds[(i/3)-1].x;
		pnts[i].u = u*10;
		pntsAtt();
		if (i%3 == 0 && (i>0)) bezier(pnts[i-3].x, pnts[i-3].y, pnts[i-2].x, pnts[i-2].y, pnts[i-1].x, pnts[i-1].y, pnts[i].x, pnts[i].y, u);
	}
	if(fechada && qnt_pontos != 0){
		qnt_pontos-=3;
	}

	//Mostra algoritmo de calculo de tangente atual
	if (tangente) glColor3f(0.0f, 1.0f, 0.0f);
	else glColor3f(1.0f, 1.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2i(10, 20);
		glVertex2i(10, 10);
		glVertex2i(20, 10);
		glVertex2i(20, 20);
	glEnd();

	//Mostra barra de slide
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2i(0, 550);
		glVertex2i(0, 500);
		glVertex2i(960, 500);
		glVertex2i(960, 550);
	glEnd();

	//Mostra controles de slide
	for (int i=0; i<qnt_slides; ++i){
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2i(slds[i].x, 550);
			glVertex2i(slds[i].x, 500);
			glVertex2i(slds[i].x+15, 500);
			glVertex2i(slds[i].x+15, 550);
		glEnd();
	}

	glFlush();
}

void handleMouse(int btn, int state, int x, int y){
	//Deleta o ponto clicando com o botao direito do mouse
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
		for(int i=0; i<qnt_pontos; ++i){
			if(pnts[i].x-4 < x && pnts[i].x+4 > x && pnts[i].y-4 < y && pnts[i].y+4 > y && i%3 == 0){
				for(int j=i; j<qnt_pontos-1; ++j){
					if (i != 0) pnts[j-2] = pnts[j+1];
					else pnts[j] = pnts[j+3];
				}
				for(int j=(i/3)-1; j<qnt_slides-1; ++j){
					slds[j] = slds[j+1];
				}
				//qnt_slides--;
				qnt_pontos-=3;
				break;
			}
		}
	}
	//Adiciona o ponto clicando com o botao esquerdo do mouse
	if(btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN && y<500 && x<960){
		//Chega se mouse esta em algum ponto de controle
		bool free = true;
		for(int i=0; i<qnt_pontos; ++i){
			if(pnts[i].x-4 < x && pnts[i].x+4 > x && pnts[i].y-4 < y && pnts[i].y+4 > y){
				free = false;
				break;
			}
		}
		if (free){
			if (qnt_pontos != 0){
				gerarPontos(x, y);
				if (qnt_slides == 0) slds[qnt_slides++] = Slide(SLD_MAX/2);
				else slds[qnt_slides++] = Slide((SLD_MAX+slds[qnt_slides-1].x)/2);
			}
			pnts[qnt_pontos++] = Ponto(x, y);
		}
	}
	//Mudar estado de movendo para -1
	if((btn == GLUT_LEFT_BUTTON && state == GLUT_UP) || (btn == GLUT_RIGHT_BUTTON && state == GLUT_UP)){
		movendo = -1;
		sliding = -1;
	}
	pntsAtt();
}

void handleMotion(int x, int y){
	if (movendo != -1){
		pnts[movendo].x = x;
		pnts[movendo].y = y;
	} else if(sliding != -1 && x>=0 && x<=950){
		if ((sliding == 0 && slds[sliding+1].x-10 >= x) || (sliding == qnt_slides-1 && slds[sliding-1].x+10 < x)) slds[sliding].x = x;
		else if (slds[sliding-1].x+10 < x && slds[sliding+1].x-10 >= x){
			slds[sliding].x = x;
		}
	} else {
		for(int i=0; i<qnt_pontos; ++i){
			if(pnts[i].x-4 < x && pnts[i].x+4 > x && pnts[i].y-4 < y && pnts[i].y+4 > y && (i%3 == 0 || i==0)){
				movendo = i;
				break;
			}
		}
		for(int i=0; i<qnt_slides; ++i){
			if(slds[i].x < x && slds[i].x+15 > x){
				sliding = i;
				break;
			}
		}
	}
	//Atualizar a tela
	pntsAtt();
	glutPostRedisplay();
}

void hadleKeyboard(unsigned char key, int x, int y){
	//Fecha aplicacao ao apertar ESC
	if(key == ESC){
		exit(0);
	}
}

void hadleSpecialKeyboard(int key, int x, int y){
	//Reinicia ao apertar F5
	if(key == GLUT_KEY_F5){
		init();
	}
	//Mostrar ou ocultar poligonais de controle
	if(key == GLUT_KEY_F4){
		showPoli = !showPoli;
	}
	//Curva fechada
	if(key == GLUT_KEY_F3){
		fechada = !fechada;
	}
	//Escolher calculo da tangente
	if(key == GLUT_KEY_F2){
		tangente = !tangente;
	}
	//Atualizar a tela
	pntsAtt();
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(960, 550);
	glutCreateWindow("BSplineInterpolation");

	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(handleMouse);
	glutMotionFunc(handleMotion);
	glutKeyboardUpFunc(hadleKeyboard);
	glutSpecialUpFunc(hadleSpecialKeyboard);

	init();

	glutMainLoop();
	return 0;
}